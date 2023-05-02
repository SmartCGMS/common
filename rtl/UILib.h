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
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
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

#include "../iface/UIIface.h"

#include <map>
#include <vector>
#include <functional>

namespace scgms {
	std::vector<TModel_Descriptor> get_model_descriptor_list();
	std::vector<TMetric_Descriptor> get_metric_descriptor_list();
	std::vector<TSolver_Descriptor> get_solver_descriptor_list();

	bool get_model_descriptor_by_id(const GUID &id, TModel_Descriptor &desc);
	bool get_model_descriptor_by_signal_id(const GUID &signal_id, TModel_Descriptor &desc);    
    bool get_signal_descriptor_by_id(const GUID& signal_id, TSignal_Descriptor& desc);
	bool get_metric_descriptor_by_id(const GUID& signal_id, TMetric_Descriptor& desc);
	bool get_solver_descriptor_by_id(const GUID& signal_id, TSolver_Descriptor& desc);

	extern const std::array<const wchar_t*, static_cast<size_t>(scgms::NDevice_Event_Code::count)> event_code_text;
	
	class CSignal_Description {
			//should we replace this conversion_factor class with a simple function, the map would have to use TBB allocator to avoid memory leaks
			//and because we don't want TBB to be a required component to compile all filters, we rather ask the programmer to instantitate
			//this class and disposes once unneeded to prevent memory leaks
	protected:
		std::map<GUID, TSignal_Descriptor> mSignal_Descriptors;
		std::map<GUID, GUID> mSignal_Reference_Signal_Ids;
        std::vector<std::wstring> mVirtual_Signal_Names;
	public:
		CSignal_Description();
		std::wstring Get_Name(const GUID &signal_id) const;
        bool Get_Descriptor(const GUID& signal_id, TSignal_Descriptor &desc) const;
		bool Get_Reference_Signal_Id(const GUID& signal_id, GUID& reference_id) const;

        void for_each(std::function<void(scgms::TSignal_Descriptor)> callback) const;
	};

	size_t Segment_Count(const size_t parameters_count, const TModel_Descriptor& desc);	//non-zero parameters_count and zero result means corrupted data

	// checks for flags presence (all-or-none)
	template<typename TFlags>
	inline bool Has_Flags_All(const TFlags& field, const TFlags& check_flags) {
		return (field & check_flags) == check_flags;
	}

	// check for flags presence (any one of given flags)
	template<typename TFlags>
	inline bool Has_Flags_Any(const TFlags& field, const TFlags& check_flags) {
		return (field & check_flags) != 0;
	}
}
