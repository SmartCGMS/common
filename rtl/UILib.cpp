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

#include "UILib.h"
#include "scgmsLib.h"

#include "../lang/dstrings.h"
#include "../utils/winapi_mapping.h"
#include "../utils/string_utils.h"

#include <map>
#include <wchar.h>


namespace scgms {
	namespace imported {
		scgms::TGet_Metric_Descriptors get_metric_descriptors = scgms::factory::resolve_symbol<scgms::TGet_Metric_Descriptors>("get_metric_descriptors");
		scgms::TGet_Model_Descriptors get_model_descriptors = scgms::factory::resolve_symbol<scgms::TGet_Model_Descriptors>("get_model_descriptors");
		scgms::TGet_Solver_Descriptors get_solver_descriptors = scgms::factory::resolve_symbol<scgms::TGet_Solver_Descriptors>("get_solver_descriptors");
		scgms::TGet_Signal_Descriptors get_signal_descriptors = scgms::factory::resolve_symbol<scgms::TGet_Signal_Descriptors>("get_signal_descriptors");
	}

	std::vector<TModel_Descriptor> get_model_descriptors()
	{
		std::vector<TModel_Descriptor> result;
		TModel_Descriptor *desc_begin, *desc_end;

		if (imported::get_model_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(result));
		}

		return result;
	}

	std::vector<TMetric_Descriptor> get_metric_descriptors()
	{
		std::vector<TMetric_Descriptor> result;
		TMetric_Descriptor *desc_begin, *desc_end;

		if (imported::get_metric_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(result));
		}

		return result;
	}

	std::vector<TSolver_Descriptor> get_solver_descriptors()
	{
		std::vector<TSolver_Descriptor> result;
		TSolver_Descriptor *desc_begin, *desc_end;

		if (imported::get_solver_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(result));
		}

		return result;
	}

	template <typename TGet_Descriptors, typename TDescriptor>
	bool get_descriptor_by_id(const GUID& id, TDescriptor& desc, TGet_Descriptors get_descriptors) {
		TDescriptor* desc_begin, * desc_end;

		bool result = get_descriptors(&desc_begin, &desc_end) == S_OK;
		if (result) {
			result = false;	//we have to find the filter yet
			for (auto iter = desc_begin; iter != desc_end; iter++)
				if (iter->id == id) {
					//desc = *iter;							assign const won't work with const members and custom operator= will result into undefined behavior as it has const members (so it does not have to be const itself)
					memcpy(&desc, iter, sizeof(decltype(desc)));	//=> memcpy https://stackoverflow.com/questions/9218454/struct-with-const-member
					result = true;
					break;
				}
		}

		return result;
	}

	bool get_model_descriptor_by_id(const GUID& id, TModel_Descriptor& desc) {
		return get_descriptor_by_id<TGet_Model_Descriptors, TModel_Descriptor>(id, desc, imported::get_model_descriptors);
	}


	bool get_model_descriptor_by_signal_id(const GUID &signal_id, TModel_Descriptor &desc) {
		TModel_Descriptor *desc_begin, *desc_end;

		bool result = imported::get_model_descriptors(&desc_begin, &desc_end) == S_OK;
		if (result) {
			result = false;	//we have to find the filter yet
			for (auto iter = desc_begin; iter != desc_end; iter++)
			{
				for (size_t i = 0; i < iter->number_of_calculated_signals; i++)
				{
					if (iter->calculated_signal_ids[i] == signal_id) {
						//desc = *iter;							assign const won't work with const members and custom operator= will result into undefined behavior as it has const members (so it does not have to be const itself)
						memcpy(&desc, iter, sizeof(decltype(desc)));	//=> memcpy https://stackoverflow.com/questions/9218454/struct-with-const-member
						result = true;
						break;
					}
				}
			}
		}

		return result;
	}


	bool get_signal_descriptor_by_id(const GUID& signal_id, TSignal_Descriptor& desc) {
		return get_descriptor_by_id<TGet_Signal_Descriptors, TSignal_Descriptor>(signal_id, desc, imported::get_signal_descriptors);
	}

	bool get_metric_descriptor_by_id(const GUID& signal_id, TMetric_Descriptor& desc) {
		return get_descriptor_by_id<TGet_Metric_Descriptors, TMetric_Descriptor>(signal_id, desc, imported::get_metric_descriptors);
	}

	bool get_solver_descriptor_by_id(const GUID& signal_id, TSolver_Descriptor& desc) {
		return get_descriptor_by_id<TGet_Solver_Descriptors, TSolver_Descriptor>(signal_id, desc, imported::get_solver_descriptors);
	}

	const std::array<const wchar_t*, static_cast<size_t>(scgms::NDevice_Event_Code::count)> event_code_text = { {
		L"Nothing",
		L"Shut_Down",
		L"Level",
		L"Masked_Level",
		L"Parameters",
		L"Parameters_Hint",
		L"Suspend_Parameter_Solving",
		L"Resume_Parameter_Solving",
		L"Solve_Parameters",
		L"Time_Segment_Start",
		L"Time_Segment_Stop",
		L"Warm_Reset",
		L"Information",
		L"Warning",
		L"Error"
	} };


	CSignal_Description::CSignal_Description() {
		mSignal_Descriptors.clear();

		TSignal_Descriptor *desc_begin, *desc_end;
		if (imported::get_signal_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::transform(desc_begin, desc_end, std::inserter(mSignal_Descriptors, mSignal_Descriptors.end()), [](const TSignal_Descriptor& desc) {return std::make_pair(desc.id, desc); });
		}

		for (size_t i = 0; i < scgms::signal_Virtual.size(); i++) {
			std::wstring desc_str = dsSignal_Prefix_Virtual + std::wstring(L" ") + std::to_wstring(i);

			TSignal_Descriptor desc{ scgms::signal_Virtual[i],
									desc_str.c_str(),
									L"",
									NSignal_Unit::Other,
									0,
									0,
									NSignal_Visualization::smooth,
									NSignal_Mark::none,
								    nullptr
			};
						
			std::transform(&desc, (&desc) + 1, std::inserter(mSignal_Descriptors, mSignal_Descriptors.end()), [](const TSignal_Descriptor& desc) {return std::make_pair(desc.id, desc); });
			mVirtual_Signal_Names.push_back(std::move(desc_str));	//move to retain valid pointer
		}


		auto describe_special_signal = [this](const GUID id, const wchar_t* desc_str)
		{
			TSignal_Descriptor desc{
				id,
				desc_str,
				L"",
				NSignal_Unit::Other,
				0,
				0,
				NSignal_Visualization::mark,
				NSignal_Mark::none,
				nullptr
			};

			std::transform(&desc, (&desc) + 1, std::inserter(mSignal_Descriptors, mSignal_Descriptors.end()), [](const TSignal_Descriptor& desc) {return std::make_pair(desc.id, desc); });
		};
		
		describe_special_signal(scgms::signal_All, dsSignal_GUI_Name_All);
		describe_special_signal(scgms::signal_Null, dsSignal_Null);
		describe_special_signal(Invalid_GUID, dsInvalid_ID);
	
	}

	std::wstring CSignal_Description::Get_Name(const GUID &signal_id) const {
		const auto result = mSignal_Descriptors.find(signal_id);
		if (result != mSignal_Descriptors.end()) return result->second.signal_description;
			else return GUID_To_WString(signal_id);
	}

	bool CSignal_Description::Get_Descriptor(const GUID& signal_id, TSignal_Descriptor& desc) const {
		const auto result = mSignal_Descriptors.find(signal_id);
		if (result != mSignal_Descriptors.end()) {
			memcpy(&desc, &(result->second), sizeof(decltype(desc)));
			return true;
		} else 
			return false;
	}

	void CSignal_Description::for_each(std::function<void(scgms::TSignal_Descriptor)> callback) const {
		for (const auto& elem : mSignal_Descriptors) {
			callback(elem.second);
		}
	}



	size_t Segment_Count(const size_t parameters_count, const TModel_Descriptor& desc) {
		if (parameters_count == 0)
			return 0;

		if (desc.number_of_segment_specific_parameters == 0)
			return parameters_count == desc.total_number_of_parameters ? 1 : 0;

		const size_t segment_agnostic_parameters = desc.total_number_of_parameters - desc.number_of_segment_specific_parameters;
		const size_t total_specific_count = parameters_count - segment_agnostic_parameters;
		const size_t reminder = total_specific_count % desc.number_of_segment_specific_parameters;
		if (reminder != 0)
			return 0;		//check failed, the reminder must be positive

		return reminder = total_specific_count / desc.number_of_segment_specific_parameters;
	}

}