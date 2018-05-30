#include "UILib.h"
#include "winapi_mapping.h"
#include <wchar.h>
#include <map>

namespace glucose
{
	namespace imported {
		//#define DIMPORT_TEST_FAIL E_NOTIMPL

		#ifdef DIMPORT_TEST_FAIL

			HRESULT IfaceCalling get_metric_descriptors(glucose::TMetric_Descriptor **begin, glucose::TMetric_Descriptor **end) {
				return DIMPORT_TEST_FAIL;
			}

			HRESULT IfaceCalling get_model_descriptors(glucose::TModel_Descriptor **begin, glucose::TModel_Descriptor **end) {
				return DIMPORT_TEST_FAIL;
			}

			HRESULT IfaceCalling get_solver_descriptors(glucose::TSolver_Descriptor **begin, glucose::TSolver_Descriptor **end) {
				return DIMPORT_TEST_FAIL;
			}

		#else
			#ifdef _WIN32
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_metric_descriptors(glucose::TMetric_Descriptor **begin, glucose::TMetric_Descriptor **end);
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_model_descriptors(glucose::TModel_Descriptor **begin, glucose::TModel_Descriptor **end);
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_solver_descriptors(glucose::TSolver_Descriptor **begin, glucose::TSolver_Descriptor **end);
			#else
				extern "C" HRESULT IfaceCalling get_metric_descriptors(glucose::TMetric_Descriptor **begin, glucose::TMetric_Descriptor **end);
				extern "C" HRESULT IfaceCalling get_model_descriptors(glucose::TModel_Descriptor **begin, glucose::TModel_Descriptor **end);
				extern "C" HRESULT IfaceCalling get_solver_descriptors(glucose::TSolver_Descriptor **begin, glucose::TSolver_Descriptor **end);
			#endif
		#endif
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
		L"Calibrated",
		L"Parameters",
		L"Parameters_Hint",
		L"Suspend_Parameter_Solving",
		L"Resume_Parameter_Solving",
		L"Solve_Parameters",
		L"Time_Segment_Start",
		L"Time_Segment_Stop",
		L"Simulation_Step",
		L"Information",
		L"Warning",
		L"Error"
	};


	std::wstring Signal_Id_To_WStr(const GUID &signal_id) {
		const static std::map<GUID, const wchar_t*> signal_names = {
			{ signal_BG, L"BG" },
			{ signal_IG, L"IG" },
			{ signal_ISIG, L"ISIG" },
			{ signal_Calibration, L"Calibration" },
			{ signal_Insulin, L"Insulin" },
			{ signal_Carb_Intake, L"Carb" },
			{ signal_Health_Stress, L"Stress" },
			{ signal_Diffusion_v2_Blood, L"Diff2 BG" },
			{ signal_Diffusion_v2_Ist, L"Diff2 IG" },
			{ signal_Steil_Rebrin_Blood, L"SR BG" }
		};

		const auto resolved_name = signal_names.find(signal_id);
		if (resolved_name != signal_names.end()) return resolved_name->second;
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
