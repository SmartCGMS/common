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
