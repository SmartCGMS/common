#pragma once

#include "../iface/UIIface.h"

GUID WString_To_GUID(const std::wstring& str);
std::wstring GUID_To_WString(const GUID &guid);
std::wstring Get_Padded_Number(uint32_t num, size_t places);
