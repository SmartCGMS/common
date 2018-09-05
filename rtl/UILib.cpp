/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

#include "UILib.h"
#include "winapi_mapping.h"
#include "FactoryLib.h"

#include <map>
#include <wchar.h>
#include "../lang/dstrings.h"

namespace glucose
{
	namespace imported {
		glucose::TGet_Metric_Descriptors get_metric_descriptors = factory::resolve_symbol<glucose::TGet_Metric_Descriptors>("get_metric_descriptors");
		glucose::TGet_Model_Descriptors get_model_descriptors = factory::resolve_symbol<glucose::TGet_Model_Descriptors>("get_model_descriptors");
		glucose::TGet_Solver_Descriptors get_solver_descriptors = factory::resolve_symbol<glucose::TGet_Solver_Descriptors>("get_solver_descriptors");		
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

	bool get_model_descriptor_by_id(const GUID &id, TModel_Descriptor &desc) {
		TModel_Descriptor *desc_begin, *desc_end;

		bool result = imported::get_model_descriptors(&desc_begin, &desc_end) == S_OK;
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


	const std::array<const wchar_t*, static_cast<size_t>(glucose::NDevice_Event_Code::count)> event_code_text = {
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
		L"Simulation_Step",
		L"Information",
		L"Warning",
		L"Error"
	};


	
	CSignal_Names::CSignal_Names() {
		mSignal_Names.clear();

		mSignal_Names[glucose::signal_All] = dsSignal_GUI_Name_All;
		mSignal_Names[glucose::signal_BG] = dsSignal_GUI_Name_BG;
		mSignal_Names[glucose::signal_IG] = dsSignal_GUI_Name_IG;
		mSignal_Names[glucose::signal_ISIG] = dsSignal_GUI_Name_ISIG;
		mSignal_Names[glucose::signal_Calibration] = dsSignal_GUI_Name_Calibration;
		mSignal_Names[glucose::signal_Insulin] = dsSignal_GUI_Name_Insulin;
		mSignal_Names[glucose::signal_Carb_Intake] = dsSignal_GUI_Name_Carbs;
		mSignal_Names[glucose::signal_Health_Stress] = dsSignal_GUI_Name_Stress;

		auto models = glucose::get_model_descriptors();
		for (auto& model : models)
		{
			for (size_t i = 0; i < model.number_of_calculated_signals; i++)
				mSignal_Names[model.calculated_signal_ids[i]] = std::wstring{ model.description } +std::wstring{ L" - " } +model.calculated_signal_names[i];
		}

		for (size_t i = 0; i < glucose::signal_Virtual.size(); i++)
			mSignal_Names[glucose::signal_Virtual[i]] = dsSignal_Prefix_Virtual + std::wstring(L" ") + std::to_wstring(i);
	}

	std::wstring CSignal_Names::Get_Name(const GUID &signal_id) {
		const auto result = mSignal_Names.find(signal_id);
		if (result != mSignal_Names.end()) return result->second;
			else return GUID_To_WString(signal_id);
	}

}

GUID WString_To_GUID(const std::wstring& str) {
	GUID guid;

	swscanf_s(str.c_str(), 
		L"{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
		&guid.Data1, &guid.Data2, &guid.Data3,
		&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
		&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]);

	return guid;
}

std::wstring GUID_To_WString(const GUID &guid) {
	const size_t guid_len = 39;
	wchar_t guid_cstr[guid_len];
	swprintf(guid_cstr, guid_len,
		L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return std::wstring(guid_cstr);
}

std::wstring Get_Padded_Number(uint32_t num, size_t places)
{
	std::wstring tmp = std::to_wstring(num);
	while (tmp.length() < places)
		tmp = L'0' + tmp;
	return tmp;
}
