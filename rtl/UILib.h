#pragma once

#include "../iface/UIIface.h"

GUID WString_To_GUID(const std::wstring& str);
std::wstring GUID_To_WString(const GUID &guid);