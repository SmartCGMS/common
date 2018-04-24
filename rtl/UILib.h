#pragma once

#include "../iface/UIIface.h"

namespace glucose
{
	std::vector<TModel_Descriptor> get_model_descriptors();
	std::vector<TMetric_Descriptor> get_metric_descriptors();
	std::vector<TSolver_Descriptor> get_solver_descriptors();

	bool get_model_descriptor_by_id(const GUID &id, TModel_Descriptor &desc);
	bool get_model_descriptor_by_signal_id(const GUID &signal_id, TModel_Descriptor &desc);


	constexpr GUID Drawing_Filter = { 0x850a122c, 0x8943, 0xa211,{ 0xc5, 0x14, 0x25, 0xba, 0xa9, 0x14, 0x35, 0x74 } };
	constexpr GUID Log_Filter = { 0xc0e942b9, 0x3928, 0x4b81,{ 0x9b, 0x43, 0xa3, 0x47, 0x66, 0x82, 0x0, 0xBA } };

}

GUID WString_To_GUID(const std::wstring& str);
std::wstring GUID_To_WString(const GUID &guid);
std::wstring Get_Padded_Number(uint32_t num, size_t places);
