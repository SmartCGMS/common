#pragma once

#include "../iface/UIIface.h"

namespace glucose
{
	std::vector<TModel_Descriptor> get_model_descriptors();
	std::vector<TMetric_Descriptor> get_metric_descriptors();
	std::vector<TSolver_Descriptor> get_solver_descriptors();

	bool get_model_descriptors_by_id(const GUID &id, TModel_Descriptor &desc);
}

GUID WString_To_GUID(const std::wstring& str);
std::wstring GUID_To_WString(const GUID &guid);
std::wstring Get_Padded_Number(uint32_t num, size_t places);
