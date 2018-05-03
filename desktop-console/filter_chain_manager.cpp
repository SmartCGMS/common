#include "filter_chain_manager.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "../rtl/DbLib.h"
#include "qdb_connector.h"

CFilter_Chain_Manager::CFilter_Chain_Manager()
{
	//
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

	//verify, that the chain ends with /dev/null filter
	bool add_dev_null = true;
	if (!mFilterChain.empty()) {
		add_dev_null = mFilterChain[mFilterChain.size() - 1].descriptor.id != glucose::Dev_NULL_Filter;
	}
	if (add_dev_null) {
		glucose::TFilter_Descriptor dev_null_desc{ 0 };
		if (glucose::get_filter_descriptors_by_id(glucose::Dev_NULL_Filter, dev_null_desc)) {
			mFilterChain.push_back(TFilter_Chain_Link{ dev_null_desc , CFilter_Configuration{} });
		}
		
	}

	// create pipes
	size_t i;
	for (i = 0; i < mFilterChain.size() + 1; i++)
		mFilterPipes.push_back(glucose::create_filter_pipe());

	// create filters
	i = 1;
	for (auto& filt : mFilterChain)
	{
		// attempt to create filter; supply proper pipes
		auto filter = glucose::create_filter(filt.descriptor.id, mFilterPipes[i - 1], mFilterPipes[i]);
		if (!filter)
		{
			//std::wcerr << "ERROR: could not create filter " << filt.descriptor.description << std::endl;
			return ENODEV;
		}

		{
			db::SDb_Sink db_sink;
			refcnt::Query_Interface<glucose::IFilter, db::IDb_Sink>(filter.get(), db::Db_Sink_Filter, db_sink);
			if (db_sink) db_sink->Set_Connector(static_cast<db::IDb_Connector*>(&db_connector));
		}

		auto params = refcnt::Create_Container_shared<glucose::TFilter_Parameter>(filt.configuration.data(), filt.configuration.data() + filt.configuration.size());

		// configure filter using loaded configuration and start the filter thread
		mFilterThreads.push_back(std::make_unique<std::thread>([params, filter, &filt]() {
			HRESULT hres = filter->Run(params.get());
			//if (hres != S_OK)
			//	std::wcerr << "ERROR: could not configure filter " << filt.descriptor.description << ", error: " << hres << std::endl;
		}));

		// add filter to vector
		mFilters.push_back(filter);
		//std::wcerr << "#" << i << ": " << filt.descriptor.description << std::endl;

		i++;
	}

	return S_OK;
}

HRESULT CFilter_Chain_Manager::Terminate_Filters() {
	if (!mFilterPipes.empty()) {
		// at first, call abort on pipes - this causes threads blocked on pop/push to unblock and return
		const glucose::TDevice_Event shut_down_event{ glucose::NDevice_Event_Code::Shut_Down };
		mFilterPipes[0]->send(&shut_down_event);


		//for (auto &pipe : mFilterPipes)
	//		pipe->abort();

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
