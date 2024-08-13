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
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#pragma once

#include "../iface/UIIface.h"

#include <map>
#include <vector>
#include <functional>

namespace scgms {

	/* retrieves a vector of loaded model descriptors */
	std::vector<TModel_Descriptor> get_model_descriptor_list();
	/* retrieves a vector of loaded metric descriptors */
	std::vector<TMetric_Descriptor> get_metric_descriptor_list();
	/* retrieves a vector of loaded solver descriptors */
	std::vector<TSolver_Descriptor> get_solver_descriptor_list();

	/* retrieves a model descriptor of a given GUID */
	bool get_model_descriptor_by_id(const GUID &id, TModel_Descriptor &desc);
	/* retrieves a model descriptor of a model producing a given signal GUID */
	bool get_model_descriptor_by_signal_id(const GUID &signal_id, TModel_Descriptor &desc);    
	/* retrieves a signal descriptor of a given GUID */
	bool get_signal_descriptor_by_id(const GUID& signal_id, TSignal_Descriptor& desc);
	/* retrieves a metric descriptor of a given GUID */
	bool get_metric_descriptor_by_id(const GUID& signal_id, TMetric_Descriptor& desc);
	/* retrieves a solver descriptor of a given GUID */
	bool get_solver_descriptor_by_id(const GUID& signal_id, TSolver_Descriptor& desc);

	/* array of string representations of event codes */
	extern const std::array<const wchar_t*, static_cast<size_t>(scgms::NDevice_Event_Code::count)> event_code_text;
	
	/* a container class of all loaded signals described by a descriptor */
	class CSignal_Description {
		protected:
			std::map<GUID, TSignal_Descriptor> mSignal_Descriptors;
			std::map<GUID, GUID> mSignal_Reference_Signal_Ids;
			std::vector<std::wstring> mVirtual_Signal_Names;

		public:
			CSignal_Description();
			/* retrieve a signal name by its GUID */
			std::wstring Get_Name(const GUID &signal_id) const;
			/* retrueve a signal descriptor by its GUID */
			bool Get_Descriptor(const GUID& signal_id, TSignal_Descriptor &desc) const;
			/* retrieves a reference signal GUID by a source signal GUID */
			bool Get_Reference_Signal_Id(const GUID& signal_id, GUID& reference_id) const;

			/* traverses the signal descriptors and invokes a callback on each of them */
			void for_each(std::function<void(scgms::TSignal_Descriptor)> callback) const;
	};

	/* extracts segment count from given parameter count */
	size_t Segment_Count(const size_t parameters_count, const TModel_Descriptor& desc);

	/* checks for flags presence (all-or-none) */
	template<typename TFlags>
	inline bool Has_Flags_All(const TFlags& field, const TFlags& check_flags) {
		return (field & check_flags) == check_flags;
	}

	/* check for flags presence (any one of given flags) */
	template<typename TFlags>
	inline bool Has_Flags_Any(const TFlags& field, const TFlags& check_flags) {
		return (field & check_flags) != 0;
	}
}
