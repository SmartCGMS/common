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

#pragma once

#include "../rtl/guid.h"
#include "winapi_mapping.h"

#include <string>
#include <locale>
#include <vector>

 // compile-time type+value selector - ending part
template<typename T, typename A0>
static constexpr T char_type_selector(A0 arg0)
{
    if constexpr (std::is_same_v<T, A0>)
        return arg0;
    else
        return static_cast<T>(arg0); // fallback to static cast of last parameter in pack
}

// compile-time type+value selector - recursive part
template<typename T, typename A0, typename... Args>
static constexpr T char_type_selector([[maybe_unused]] A0 arg0, Args... args)
{
    if constexpr (std::is_same_v<T, A0>)
        return arg0;
    else
        return char_type_selector<T>(args...);
}

template<typename CharT>
class CDecimal_Separator : public std::numpunct<CharT> {
protected:
	CharT mSeparator;
	CharT do_decimal_point() const { return mSeparator; }
public:
	CDecimal_Separator(CharT Separator, size_t refs = 0) : std::numpunct<CharT>(refs), mSeparator(Separator) {}
};


//works with non-locale installed languages
std::string Narrow_WString(const std::wstring &wstr);
std::string Narrow_WChar(const wchar_t *wstr);
std::wstring Widen_Char(const char *str);
std::wstring Widen_String(const std::string &str);


inline bool Is_Empty(const std::wstring& wstr) {
    return wstr.empty();
}

template <typename... Args>
bool Is_Empty(const std::wstring& wstr, const Args&... args) {
    return Is_Empty(wstr) || Is_Empty(args...);
}



double str_2_dbl(const char* str);
double str_2_dbl(const char* str, bool& ok);
double str_2_dbl(const wchar_t* wstr);
double str_2_dbl(const wchar_t* wstr, bool& ok);

std::vector<double> str_2_dbls(const wchar_t* wstr, bool& ok);

std::wstring dbl_2_wstr(const double val);

template <typename C>
std::wstring dbl_2_str(const C& container) {
    std::wstring result;
    for (auto iter = container.begin(); iter != container.end(); iter++) {
        if (!result.empty())
            result += L' ';
        result += dbl_2_wstr(*iter);
    }

    return result;
}


int64_t str_2_int(const char* str);
int64_t str_2_int(const char* str, bool& ok);
int64_t str_2_int(const std::string& str, bool& converted_ok);
int64_t str_2_int(const wchar_t* wstr);
int64_t str_2_int(const wchar_t* wstr, bool& ok);
int64_t str_2_int(const std::wstring& str, bool& converted_ok);
uint64_t str_2_uint(const wchar_t* wstr, bool& ok);

GUID WString_To_GUID(const std::wstring& str, bool& ok);
std::wstring GUID_To_WString(const GUID& guid);
std::wstring Get_Padded_Number(uint32_t num, size_t places);

bool str_2_bool(const std::wstring& str, bool& ok);


std::wstring& trim(std::wstring& str);

std::string quote(const std::string& str);


std::wstring Lower_String(const std::wstring& wstr);
std::string Lower_String(const std::string& wstr);

