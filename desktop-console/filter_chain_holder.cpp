#include "filter_chain_holder.h"

#include <iostream>
#include <thread>
#include <chrono>


CFilter_Chain_Holder::CFilter_Chain_Holder()
{
	//
}

CFilter_Chain_Holder::CFilter_Chain_Holder(CFilter_Chain& sourceChain)
	: mFilterChain(sourceChain)
{
	//
}

CFilter_Chain& CFilter_Chain_Holder::Get_Filter_Chain()
{
	return mFilterChain;
}

HRESULT CFilter_Chain_Holder::Init_And_Start_Filters()
{
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
			std::wcerr << "ERROR: could not create filter " << filt.descriptor.description << std::endl;
			return ENODEV;
		}

		auto params = refcnt::Create_Container_shared<glucose::TFilter_Parameter>(filt.configuration.data(), filt.configuration.data() + filt.configuration.size());

		// configure filter using loaded configuration and start the filter thread
		mFilterThreads.push_back(std::make_unique<std::thread>([params, filter, &filt]() {
			HRESULT hres = filter->Run(params);
			if (hres != S_OK)
				std::wcerr << "ERROR: could not configure filter " << filt.descriptor.description << ", error: " << hres << std::endl;
		}));

		// add filter to vector
		mFilters.push_back(filter);
//		std::wcerr << "#" << i << ": " << filt.descriptor.description << std::endl;

		i++;
	}

	return S_OK;
}

HRESULT CFilter_Chain_Holder::Terminate_Filters()
{
	size_t i;

	// at first, call abort on pipes - this causes threads blocked on pop/push to unblock and return
	for (i = 0; i < mFilterPipes.size(); i++)
		mFilterPipes[i]->abort();

	// join filter threads; they should exit very soon after the pipe is aborted
	for (i = 0; i < mFilterThreads.size(); i++)
	{
		if (mFilterThreads[i]->joinable())
			mFilterThreads[i]->join();
	}

	mFilters.clear();
	mFilterPipes.clear();
	mFilterThreads.clear();

	return S_OK;
}

glucose::SFilter CFilter_Chain_Holder::Get_Filter(size_t index)
{
	if (mFilters.size() <= index)
		return {};

	return mFilters[index];
}

GUID CFilter_Chain_Holder::Get_Filter_Id(size_t index) const
{
	if (mFilters.size() <= index)
		return {0};

	return mFilterChain[index].descriptor.id;
}
