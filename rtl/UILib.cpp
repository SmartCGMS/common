#include "UILib.h"


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
