/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8
 * 301 00, Pilsen
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#include "filter_chain_manager.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "../rtl/DbLib.h"
#ifndef SMARTCGMS_NO_DB
#include "qdb_connector.h"
#endif

CFilter_Chain_Manager::CFilter_Chain_Manager() noexcept {
	
}

CFilter_Chain_Manager::CFilter_Chain_Manager(CFilter_Chain& sourceChain)
	: mFilterChain(sourceChain), mConsume_Outputs(false)
{
	//
}

CFilter_Chain& CFilter_Chain_Manager::Get_Filter_Chain()
{
	return mFilterChain;
}

HRESULT CFilter_Chain_Manager::Init_And_Start_Filters(bool consumeOutputs) {
	HRESULT res;

	if (mFilterChain.empty()) return S_FALSE;

	Terminate_Filters();
	mConsume_Outputs = consumeOutputs;

	// create pipes
	size_t i;

	// this loop creates "preceding" pipe for each filter
	bool lastSync = false;
	for (i = 0; i < mFilterChain.size(); i++) {

		// consecutive synchronnous filters creates just one synchronnous pipe
		if ((mFilterChain[i].descriptor.flags & glucose::NFilter_Flags::Synchronnous) == glucose::NFilter_Flags::Synchronnous)
		{
			if (!lastSync)
			{
				lastSync = true;
				if (!add_pipe<glucose::SFilter_Synchronnous_Pipe>())
					return E_FAIL;
			}
		}
		else
		{
			// only two or more consecutive asynchronnous filters need asynchronnous pipe

			if (lastSync)
				lastSync = false;
			else
			{
				if (!add_pipe<glucose::SFilter_Asynchronnous_Pipe>())
					return E_FAIL;
			}
		}
	}

	// add chain output pipe - only if the last filter wasn't synchronnous; otherwise the synchronnous pipe is also output pipe
	if (!lastSync)
	{
		if (!add_pipe<glucose::SFilter_Asynchronnous_Pipe>())
			return E_FAIL;
	}

	// Pipe assignment rules:
	// 1) consecutive synchronnous filters are added to a single synchronnous pipe
	// 2) async filter following sync filter retains sync pipe as input
	// 3) async filter preceding sync filter retains sync pipe as output
	// 4) async filter following another async filter shares async pipe with preceding filter

	// create filters
	i = 1;
	for (auto& filt : mFilterChain)	{

		if ((filt.descriptor.flags & glucose::NFilter_Flags::Synchronnous) == glucose::NFilter_Flags::Synchronnous)
		{
			auto filter = glucose::create_synchronnous_filter(filt.descriptor.id);
			if (!filter)
				return ENODEV;

			auto params = refcnt::Create_Container_shared<glucose::TFilter_Parameter>(filt.configuration.data(), filt.configuration.data() + filt.configuration.size());

			res = filter->Configure(params.get());
			if (res != S_OK)
				return res;

			// add filter to vector
			mFilters.push_back(filter);

			// retrieve last pipe (it must be synchronnous pipe, otherwise there's a logic error in pipe create loop)
			if (!mFilterPipes[i - 1]->add_filter(filter)) return E_FAIL;
		}
		else
		{
			// attempt to create filter; supply proper pipes
			auto filter = glucose::create_asynchronnous_filter(filt.descriptor.id, mFilterPipes[i - 1]->get_raw_pipe(), mFilterPipes[i]->get_raw_pipe());
			if (!filter)
				return ENODEV;

#ifndef SMARTCGMS_NO_DB
			{
				db::SDb_Sink db_sink;
				refcnt::Query_Interface<glucose::IFilter, db::IDb_Sink>(filter.get(), db::Db_Sink_Filter, db_sink);
				if (db_sink) db_sink->Set_Connector(static_cast<db::IDb_Connector*>(&db_connector));
			}
#endif

			// add filter to vector
			mFilters.push_back(filter);

			i++;
		}
	}

	// to avoid race conditions, start asynchronnous filters after all filters are instantiated and pipes are initialized
	for (i = 0; i < mFilters.size(); i++)
	{
		auto& filter = mFilters[i];

		if ((mFilterChain[i].descriptor.flags & glucose::NFilter_Flags::Synchronnous) != glucose::NFilter_Flags::Synchronnous)
		{
			glucose::IAsynchronnous_Filter* asyncFilter = static_cast<glucose::IAsynchronnous_Filter*>(filter.get());
			if (asyncFilter)
			{
				auto params = refcnt::Create_Container_shared<glucose::TFilter_Parameter>(mFilterChain[i].configuration.data(), mFilterChain[i].configuration.data() + mFilterChain[i].configuration.size());

				// configure filter using loaded configuration and start the filter thread
				mFilterThreads.push_back(std::make_unique<std::thread>([params, asyncFilter]() {
					asyncFilter->Run(params.get());
				}));
			}
		}
	}

	// the default behaviour is to consume outputs of the last pipe, so the chain does not get stuck
	// when the queue reaches its limit; we may, however, not want to consume outputs and call Receive by ourselves
	// - this is typically used in any kind of subchains
	if (mConsume_Outputs)
	{
		// the consume thread is considered a filter thread; it terminates with Shut_Down message
		mFilterThreads.push_back(std::make_unique<std::thread>([this]() {
			auto input = mFilterPipes[mFilterPipes.size() - 1].get();

			for (; glucose::UDevice_Event evt = input->Receive(); ) {
				// just read and do nothing - this effectively consumes any incoming event through pipe

				//and if it was a shutdown event, try to repost it into the first filter
				//in the case, that some filter in the middle had produced the event
				//then, we need to make sure that all preceding filters terminate as well
				if (evt)
					if (evt.event_code() == glucose::NDevice_Event_Code::Shut_Down)
						mFilterPipes[0]->Send(evt);	// no need to test success
			}
		}));
	}

	return S_OK;
}

HRESULT CFilter_Chain_Manager::Terminate_Filters() {
	if (!mFilterPipes.empty()) {
		// send Shut_Down event through the chain - pipes inside the chain will abort, causing filters to shut down on next Send or Receive call
		glucose::UDevice_Event shut_down_event{ glucose::NDevice_Event_Code::Shut_Down };
		mFilterPipes[0]->Send(shut_down_event);

		// join filter threads; they should shut down very soon after the pipe is aborted
		Join_Filters();

		mFilterThreads.clear();
		mFilters.clear();
		mFilterPipes.clear();
	}

	return S_OK;
}

HRESULT CFilter_Chain_Manager::Join_Filters() {
	for (auto &filter_thread : mFilterThreads) {
		if (filter_thread->joinable())
			filter_thread->join();
	}

	return S_OK;
}

HRESULT CFilter_Chain_Manager::Send(glucose::UDevice_Event &event) {
	if (mFilterPipes.empty()) return S_FALSE;
	return mFilterPipes[0]->Send(event) ? S_OK : E_FAIL;
}

glucose::UDevice_Event CFilter_Chain_Manager::Receive() {
	if (mConsume_Outputs || mFilterPipes.empty()) return glucose::UDevice_Event{};
	return mFilterPipes[mFilterPipes.size() - 1]->Receive();
}

void CFilter_Chain_Manager::Traverse_Filters(std::function<bool(glucose::SFilter&)> fnc) {
	for (auto& filter : mFilters) {
		if (!fnc(filter))
			break;
	}
}
