#pragma once

#include "../rtl/referencedImpl.h"
#include "filter_chain.h"
#include "../rtl/hresult.h"
#include "../rtl/FilterLib.h"

#include <thread>
#include <memory>

/*
 * Filter chain holder class - holds filter chain and manages chain running
 */
class CFilter_Chain_Holder : public virtual refcnt::CReferenced
{
	private:
		// filter chain (to be loaded using configurator)
		CFilter_Chain mFilterChain;

		// instantiated filters
		std::vector<glucose::SFilter> mFilters;
		// filter threads
		std::vector<std::unique_ptr<std::thread>> mFilterThreads;
		// instantiated filter pipes (including the "border" ones)
		std::vector<glucose::SFilter_Pipe> mFilterPipes;

	public:
		CFilter_Chain_Holder();
		CFilter_Chain_Holder(CFilter_Chain& sourceChain);

		// retrieves filter chain (i.e. to be filled by configurator)
		CFilter_Chain& Get_Filter_Chain();
		// initializes and starts the filter chain
		HRESULT Init_And_Start_Filters();
		// terminates filters
		HRESULT Terminate_Filters();
		// retrieves filter by its position
		glucose::SFilter Get_Filter(size_t index);
		// retrieves filter GUID by its position
		GUID Get_Filter_Id(size_t index) const;
};
