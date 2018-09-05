/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

#include "filter_chain_manager.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "../rtl/DbLib.h"
#include "qdb_connector.h"

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
	if (mFilterChain.empty()) return S_FALSE;

	Terminate_Filters();
	mConsume_Outputs = consumeOutputs;

	// create pipes
	size_t i;
	for (i = 0; i < mFilterChain.size() + 1; i++) {
		glucose::SFilter_Pipe pipe{};
		if (!pipe)
			return E_FAIL;
		mFilterPipes.push_back(std::move(pipe));
	}

	// create filters
	i = 1;
	for (auto& filt : mFilterChain)	{
		// attempt to create filter; supply proper pipes
		auto filter = glucose::create_filter(filt.descriptor.id, mFilterPipes[i - 1], mFilterPipes[i]);
		if (!filter) 
			return ENODEV;

		{
			db::SDb_Sink db_sink;
			refcnt::Query_Interface<glucose::IFilter, db::IDb_Sink>(filter.get(), db::Db_Sink_Filter, db_sink);
			if (db_sink) db_sink->Set_Connector(static_cast<db::IDb_Connector*>(&db_connector));
		}

		auto params = refcnt::Create_Container_shared<glucose::TFilter_Parameter>(filt.configuration.data(), filt.configuration.data() + filt.configuration.size());

		// configure filter using loaded configuration and start the filter thread
		mFilterThreads.push_back(std::make_unique<std::thread>([params, filter]() {
			filter->Run(params.get());
		}));

		// add filter to vector
		mFilters.push_back(filter);
		i++;
	}

	// the default behaviour is to consume outputs of the last pipe, so the chain does not get stuck
	// when the queue reaches its limit; we may, however, not want to consume outputs and call Receive by ourselves
	// - this is typically used in any kind of subchains
	if (mConsume_Outputs)
	{
		// the consume thread is considered a filter thread; it terminates with Shut_Down message
		mFilterThreads.push_back(std::make_unique<std::thread>([this]() {
			glucose::UDevice_Event evt;
			auto input = mFilterPipes[mFilterPipes.size() - 1];

			for (; glucose::UDevice_Event evt = input.Receive(); ) {
				// just read and do nothing - this effectively consumes any incoming event through pipe

				//and if it was a shutdown event, try to repost it into the first filter
				//in the case, that some filter in the middle had produced the event
				//then, we need to make sure that all preceding filters terminate as well
				if (evt.event_code == glucose::NDevice_Event_Code::Shut_Down)
					mFilterPipes[0].Send(evt);	// no need to test success

			}
		}));
	}

	return S_OK;
}

HRESULT CFilter_Chain_Manager::Terminate_Filters() {
	if (!mFilterPipes.empty()) {
		// send Shut_Down event through the chain - pipes inside the chain will abort, causing filters to shut down on next Send or Receive call
		glucose::UDevice_Event shut_down_event{ glucose::NDevice_Event_Code::Shut_Down };
		mFilterPipes[0].Send(shut_down_event);

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
	return mFilterPipes[0].Send(event) ? S_OK : E_FAIL;
}

glucose::UDevice_Event CFilter_Chain_Manager::Receive() {
	if (mConsume_Outputs || mFilterPipes.empty()) return glucose::UDevice_Event{};
	return mFilterPipes[mFilterPipes.size() - 1].Receive();
}

void CFilter_Chain_Manager::Traverse_Filters(std::function<bool(glucose::SFilter)> fnc) {
	for (auto& filter : mFilters) {
		if (!fnc(filter))
			break;
	}
}
