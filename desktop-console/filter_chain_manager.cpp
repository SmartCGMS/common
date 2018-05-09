#include "filter_chain_manager.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "../rtl/DbLib.h"
#include "qdb_connector.h"
#include "consumer_filter.h"

CFilter_Chain_Manager::CFilter_Chain_Manager() noexcept {
	
}

CFilter_Chain_Manager::CFilter_Chain_Manager(CFilter_Chain& sourceChain)
	: mFilterChain(sourceChain)
{
	//
}

CFilter_Chain& CFilter_Chain_Manager::Get_Filter_Chain()
{
	return mFilterChain;
}

HRESULT CFilter_Chain_Manager::Init_And_Start_Filters() {
	if (mFilterChain.empty()) return S_FALSE;

	
	Terminate_Filters();

	// create pipes
	size_t i;
	for (i = 0; i < mFilterChain.size() + 1; i++) {
		auto pipe = glucose::create_filter_pipe();
		if (!pipe) return E_FAIL;
		mFilterPipes.push_back(pipe);
	}

	// create filters
	i = 1;
	for (auto& filt : mFilterChain)	{
		// attempt to create filter; supply proper pipes
		auto filter = glucose::create_filter(filt.descriptor.id, mFilterPipes[i - 1], mFilterPipes[i]);
		if (!filter) return ENODEV;

		{
			db::SDb_Sink db_sink;
			refcnt::Query_Interface<glucose::IFilter, db::IDb_Sink>(filter.get(), db::Db_Sink_Filter, db_sink);
			if (db_sink) db_sink->Set_Connector(static_cast<db::IDb_Connector*>(&db_connector));
		}

		auto params = refcnt::Create_Container_shared<glucose::TFilter_Parameter>(filt.configuration.data(), filt.configuration.data() + filt.configuration.size());

		// configure filter using loaded configuration and start the filter thread
		mFilterThreads.push_back(std::make_unique<std::thread>([params, filter, &filt]() {
			filter->Run(params.get());
		}));

		// add filter to vector
		mFilters.push_back(filter);
		i++;
	}

	//finally, add terminating consumer filter so that the queue does not qet stuck 
	glucose::IFilter *consumer_filter;
	Manufacture_Object<CConsumer_Filter, glucose::IFilter>(&consumer_filter, mFilterPipes[mFilterPipes.size()-1]);
	mFilters.push_back(refcnt::make_shared_reference_ext<glucose::SFilter, glucose::IFilter>(consumer_filter, false));

	mFilterThreads.push_back(std::make_unique<std::thread>([consumer_filter]() {
		consumer_filter->Run(nullptr);
	}));

	return S_OK;
}

HRESULT CFilter_Chain_Manager::Terminate_Filters() {
	if (!mFilterPipes.empty()) {
		// at first, call abort on pipes - this causes threads blocked on pop/push to unblock and return
		const glucose::TDevice_Event shut_down_event{ glucose::NDevice_Event_Code::Shut_Down };
		mFilterPipes[0]->send(&shut_down_event);

		// join filter threads; they should exit very soon after the pipe is aborted
		for (auto &filter_thread : mFilterThreads) {
			if (filter_thread->joinable())
				filter_thread->join();
		}

		mFilters.clear();
		mFilterPipes.clear();
		mFilterThreads.clear();
	}

	return S_OK;
}

glucose::SFilter CFilter_Chain_Manager::Get_Filter(size_t index)
{
	if (mFilters.size() <= index)
		return {};

	return mFilters[index];
}

GUID CFilter_Chain_Manager::Get_Filter_Id(size_t index) const
{
	if (mFilters.size() <= index)
		return {0};

	return mFilterChain[index].descriptor.id;
}
