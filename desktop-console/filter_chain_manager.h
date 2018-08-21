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
class CFilter_Chain_Manager : public virtual refcnt::CReferenced {
	private:
		// filter chain (to be loaded using configurator)
		CFilter_Chain mFilterChain;

		// instantiated filters
		std::vector<glucose::SFilter> mFilters;
		// filter threads
		std::vector<std::unique_ptr<std::thread>> mFilterThreads;
		// instantiated filter pipes (including the "border" ones)
		std::vector<glucose::SFilter_Pipe> mFilterPipes;

		// is this filter chain initialized to consume all outputs?
		bool mConsume_Outputs = true;

	public:
		CFilter_Chain_Manager() noexcept;
		CFilter_Chain_Manager(CFilter_Chain& sourceChain);

		// initializes and starts the filter chain
		HRESULT Init_And_Start_Filters(bool consumeOutputs = true);
		// terminates filters
		HRESULT Terminate_Filters();
		// joins all filter threads
		HRESULT Join_Filters();
		// inject a specific message into the very first pipe
		HRESULT Send(glucose::UDevice_Event &event);
		// receive from last initialized pipe; this only works, when consumeOutputs flag in Init_And_Start_Filters call was set to false
		glucose::UDevice_Event Receive();

		// traverse all filters in chain; the function supplied has a return value of type bool - returns false to stop traversing ("break" traverse loop)
		void Traverse_Filters(std::function<bool(glucose::SFilter)> fnc);

		// retrieves filter chain (i.e. to be filled by configurator)
		CFilter_Chain& Get_Filter_Chain();
};
