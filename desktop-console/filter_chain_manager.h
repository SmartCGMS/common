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
class CFilter_Chain_Manager : public virtual refcnt::CReferenced
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
		CFilter_Chain_Manager() noexcept;
		CFilter_Chain_Manager(CFilter_Chain& sourceChain);

		// initializes and starts the filter chain
		HRESULT Init_And_Start_Filters();
		// terminates filters
		HRESULT Terminate_Filters();
		//inject a specific message into the very first pipe
		HRESULT Send(glucose::SDevice_Event &event);

		// retrieves filter chain (i.e. to be filled by configurator)
		CFilter_Chain& Get_Filter_Chain();
		
		
		// retrieves filter by its position
		glucose::SFilter Get_Filter(size_t index);
		// retrieves filter GUID by its position
		GUID Get_Filter_Id(size_t index) const;
};
