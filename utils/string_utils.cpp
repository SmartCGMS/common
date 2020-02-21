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
 * Univerzitni 8
 * 301 00, Pilsen
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

#include "string_utils.h"

#include <sstream>
#include <algorithm>
#include <cwctype>
#include <cstring>
#include <array>

std::string Narrow_WString(const std::wstring& wstr) {
	return Narrow_WChar(wstr.c_str());
}

std::string Narrow_WChar(const wchar_t *wstr) {
	std::ostringstream stm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet< std::ctype<wchar_t> >(stm.getloc());

	const size_t len = wcslen(wstr);
	for (size_t i = 0; i < len; ++i) 		
		stm << ctfacet.narrow(wstr[i], 0);
	
	return stm.str();
}

std::wstring Widen_Char(const char *str) {
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t> >(wstm.getloc());
	const size_t len = strlen(str);
	for (size_t i = 0; i < len; ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}


std::wstring WString_To_Lower(const std::wstring& wstr) {
	std::wstring result;

	std::transform(wstr.begin(), wstr.end(), std::back_inserter(result), std::towlower);

	return result;
}

double str_2_dbl(const char* str) {
    bool tmp;
    return str_2_dbl(str, tmp);
}

double str_2_dbl(const char *str, bool &ok) {
    char* end_char;
    double value = std::strtod(str, &end_char);
    ok = *end_char == 0;

    if (!ok) 
        value = std::numeric_limits<double>::quiet_NaN(); //sanity
        
    return value;
}


double wstr_2_dbl(const wchar_t* wstr) {
    bool tmp;
    return wstr_2_dbl(wstr, tmp);
}

double wstr_2_dbl(const wchar_t* wstr, bool& ok) {
    wchar_t* end_char;
    double value = std::wcstod(wstr, &end_char);

    ok = *end_char == 0;

    if (!ok)
        value = std::numeric_limits<double>::quiet_NaN(); //sanity

    return value;
}


template <typename T>
int get_base(T* str) {
    int base = 10;

    constexpr T lc = static_cast<T>('x');
    constexpr T up = static_cast<T>('X');
    constexpr T zr = static_cast<T>('0');

    if (str) {
        switch (str[0]) {
            case lc:
            case up:   base = 16;  
                        break;

            case zr:   switch (str[1]) {
                            case lc:
                            case up:   base = 16;
                                        break;
                            default: break;
                        }

            default: break;
        }
    }

    return base;
}


int64_t str_2_int(const char* str) {
    bool tmp;
    return str_2_int(str, tmp);
}


int64_t str_2_int(const char* str, bool &ok) {
    char* end_char;
    int64_t value = std::strtol(str, &end_char, get_base(str));
    ok = *end_char == 0;

    if (!ok)
        value = std::numeric_limits<int64_t>::max(); //sanity

    return value;
}



int64_t wstr_2_int(const wchar_t* wstr) {
    bool tmp;
    return wstr_2_int(wstr, tmp);
}


int64_t wstr_2_int(const wchar_t* wstr, bool& ok) {
    wchar_t* end_char;
    int64_t value = std::wcstol(wstr, &end_char, get_base(wstr));
    ok = *end_char == 0;

    if (!ok)
        value = std::numeric_limits<int64_t>::max(); //sanity

    return value;
}


using TGUID_Overlay = uint8_t[16];
static_assert(sizeof(GUID) == sizeof(TGUID_Overlay), "Invalid packing of GUID overlay!");
const std::array<uint8_t, 16> guid_overlay_indexes = { 3, 2, 1, 0, 5, 4, 7, 6, 8, 9, 10, 11, 12, 13, 14, 15 };	//UUID would start with 0, 1, 2, 3
const std::array<size_t, 16> guid_parsing_indexes = { 0, 2, 4, 6, 9, 11, 14, 16, 19, 21, 24, 26, 28, 30, 32, 34 };


GUID WString_To_GUID(const std::wstring& str, bool& ok) {
    GUID guid = Invalid_GUID;

    const size_t bracketless_guid_len = 36;
    const size_t str_len = str.size();
    const bool bracketed_guid = str_len == bracketless_guid_len + 2;
    ok = (str_len == bracketless_guid_len) || bracketed_guid;

    if (ok) {
        //guid str seems to have the proper size
        ok = !bracketed_guid || (bracketed_guid && (str[0] == L'{') && (str[bracketless_guid_len + 1] == L'}'));
        if (ok) {
            //if brackets are used, they are OK

            //check the hyphens
            const wchar_t* gs = str.data() + (bracketed_guid ? 1 : 0);
            const size_t h1st = 8;
            const size_t h2nd = 13;
            const size_t h3rd = 18;
            const size_t h4th = 23;
            ok = (gs[h1st] == L'-') && (gs[h2nd] == L'-') && (gs[h3rd] == L'-') && (gs[h4th] == L'-');
            if (ok) {
                auto hex_2_bin = [&ok](const wchar_t ch)->uint8_t {
                    switch (ch) {
                        case '0': case '1': case '2': case '3':	case '4': case '5':	case '6': case '7': case '8': case '9': return ch - L'0';
                        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': return ch - L'a' + 10;
                        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': return ch - L'A' + 10;
                        default: ok = false; return 0xFF;
                    }
                };

                TGUID_Overlay& guid_overlay = *reinterpret_cast<TGUID_Overlay*>(&guid);
                for (size_t i = 0; i < guid_parsing_indexes.size(); i++) {
                    const size_t idx = guid_parsing_indexes[i];
                    guid_overlay[guid_overlay_indexes[i]] = hex_2_bin(gs[idx + 0]) * 16 + hex_2_bin(gs[idx + 1]);
                }

                if (!ok) guid = Invalid_GUID;	//undo any writes that might have occured

            }
        }
    }


    return guid;
}

std::wstring GUID_To_WString(const GUID& guid) {
    std::wstring result = L"{00000000-0000-0000-0000-000000000000}";
    const wchar_t* to_x = L"0123456789ABCDEF";

    TGUID_Overlay& guid_overlay = *reinterpret_cast<TGUID_Overlay*>(const_cast<GUID*>(&guid));
    
    for (size_t i = 0; i < guid_parsing_indexes.size(); i++) {             
        
        const uint8_t byte = guid_overlay[guid_overlay_indexes[i]];
        const uint8_t lo = byte & 0xf;
        const uint8_t hi = byte / 0x10;

        const size_t result_index = guid_parsing_indexes[i];
        result[result_index + 1] = to_x[hi];        //+1 because of the starting '{'
        result[result_index + 2] = to_x[lo];
    }
    
    return result;
}

std::wstring Get_Padded_Number(uint32_t num, size_t places) {
    std::wstring tmp = std::to_wstring(num);
    while (tmp.length() < places)
        tmp = L'0' + tmp;
    return tmp;
}


const wchar_t* trim_chars = L"\t\n\v\f\r ";

std::wstring& ltrim(std::wstring& str) {
    str.erase(0, str.find_first_not_of(trim_chars));
    return str;
}

std::wstring& rtrim(std::wstring& str) {
    str.erase(str.find_last_not_of(trim_chars) + 1);
    return str;
}

std::wstring& trim(std::wstring& str) {
    return ltrim(rtrim(str));
}