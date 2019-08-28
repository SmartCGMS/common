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

#pragma once

#include "../rtl/referencedImpl.h"
#include "filter_chain.h"
#include "../rtl/hresult.h"
#include "../rtl/FilterLib.h"

#include <thread>
#include <memory>
#include <functional>

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
		std::vector<std::unique_ptr<glucose::CFilter_Pipe>> mFilterPipes;

		// is this filter chain initialized to consume all outputs?
		bool mConsume_Outputs = true;
	protected:
		template <typename S>
		bool add_pipe() {
			std::unique_ptr<glucose::CFilter_Pipe> pipe = std::make_unique<S>();
			if (pipe->empty()) return false;			

			mFilterPipes.push_back(std::move(pipe));

			return true;
		}
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
		void Traverse_Filters(std::function<bool(glucose::SFilter&)> fnc);

		// retrieves filter chain (i.e. to be filled by configurator)
		CFilter_Chain& Get_Filter_Chain();
};
