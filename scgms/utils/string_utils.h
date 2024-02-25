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
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
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

// decimal separator selector class
template<typename CharT>
class CDecimal_Separator : public std::numpunct<CharT> {
	protected:
		CharT mSeparator;
		CharT do_decimal_point() const {
			return mSeparator;
		}
	public:
		CDecimal_Separator(CharT Separator, size_t refs = 0) : std::numpunct<CharT>(refs), mSeparator(Separator) {
		}
};

// NOTE: works with non-locale installed languages

// Narrows std::wstring to std::string
std::string Narrow_WString(const std::wstring &wstr);
// narrows wchar_t-encoded string to std::string
std::string Narrow_WChar(const wchar_t *wstr);
// widens char-encoded string to std::wstring
std::wstring Widen_Char(const char *str);
// widens std::string to std::wstring
std::wstring Widen_String(const std::string &str);

// helper inline function to determine emptiness of string; also a template specialization recursion stopper
inline bool Is_Empty(const std::wstring& wstr) {
	return wstr.empty();
}

// variadic template to determine, if the set of strings are all empty
template <typename... Args>
bool Is_Empty(const std::wstring& wstr, const Args&... args) {
	return Is_Empty(wstr) || Is_Empty(args...);
}

// determines, if the string has a prefix typical for prefixing hexadecimally-encoded numbers (POD-char variant)
bool Has_Hexa_Prefix(const char* str);
bool Has_Hexa_Prefix(const std::string &str);
bool Has_Hexa_Prefix(const wchar_t* wstr);
bool Has_Hexa_Prefix(const std::wstring& wstr);

// converts string to double, considering the possibility of hexadecimal encoding
double str_2_dbl(const char* str);
double str_2_dbl(const std::string &str);
double str_2_dbl(const wchar_t* wstr);
double str_2_dbl(const std::wstring& wstr);

// converts string to double, considering the possibility of hexadecimal encoding; validates the conversion (POD-char variant)
double str_2_dbl(const char* str, bool& ok);
double str_2_dbl(const std::string& str, bool &ok);
double str_2_dbl(const wchar_t* wstr, bool& ok);
double str_2_dbl(const std::wstring& wstr, bool &ok);

// converts the input wstring containing multiple values to a vector of doubles, validates the conversion
std::vector<double> str_2_dbls(const wchar_t* wstr, bool& ok);

// converts a double value into a std::string; it either converts the double to string directly (losing precision) or dumps the double as byte array (save_as_hexa = true)
std::string dbl_2_str(const double val, const bool save_as_hexa = false);
// converts a double value into a std::wstring; it either converts the double to string directly (losing precision) or dumps the double as byte array (save_as_hexa = true)
std::wstring dbl_2_wstr(const double val, const bool save_as_hexa = false);

// converts all double values from given container to wstring; it either converts the double to string directly (losing precision) or dumps the double as byte array (save_as_hexa = true)
template <typename C>
std::wstring dbl_2_str(const C& container, const bool save_as_hexa = false) {
	std::wstring result;
	for (auto iter = container.begin(); iter != container.end(); iter++) {
		if (!result.empty()) {
			result += L' ';
		}
		result += dbl_2_wstr(*iter, save_as_hexa);
	}

	return result;
}

// converts string to integer, considering all internal rules for integer parsing
int64_t str_2_int(const char* str);
int64_t str_2_int(const std::string& str);
int64_t str_2_int(const wchar_t* wstr);
int64_t str_2_int(const std::wstring& str);

// converts string to integer, considering all internal rules for integer parsing; validates the conversion
int64_t str_2_int(const char* str, bool& ok);
int64_t str_2_int(const std::string& str, bool& ok);
int64_t str_2_int(const wchar_t* wstr, bool& ok);
int64_t str_2_int(const std::wstring& str, bool& ok);

// converts string to unsigned integer, considering all internal rules for integer parsing
uint64_t str_2_uint(const char* str);
uint64_t str_2_uint(const std::string& str);
uint64_t str_2_uint(const wchar_t* str);
uint64_t str_2_uint(const std::wstring& str);

// converts string to unsignedinteger, considering all internal rules for integer parsing; validates the conversion
uint64_t str_2_uint(const char* str, bool& ok);
uint64_t str_2_uint(const std::string& str, bool& ok);
uint64_t str_2_uint(const wchar_t* wstr, bool& ok);
uint64_t str_2_uint(const std::wstring& str, bool& ok);

// converts string to boolean flag
bool str_2_bool(const std::wstring& str, bool& ok);

// converts std::wstring to GUID; validates the conversion
GUID WString_To_GUID(const std::wstring& str, bool& ok);
// converts GUID to std::wstring
std::wstring GUID_To_WString(const GUID& guid);

// retrieves a number padded to N places with zeroes
std::wstring Get_Padded_Number(uint32_t num, size_t places);

// trims the string, returns reference to itself
std::string& trim(std::string& str);
std::wstring& trim(std::wstring& str);

// trims the string, returns a copy
std::string trim(const std::string& str);
std::string trim(const char* str);

// encloses the string into quotation marks
std::string quote(const std::string& str);

// transform string to lowercase
std::string Lower_String(const std::string& wstr);
std::wstring Lower_String(const std::wstring& wstr);

// determines, if the string ends with given substring
template <typename T>
bool ends_with(T const& complete_string, T const& ending_string) {
	if (ending_string.size() > complete_string.size()) {
		return false;
	}
	return std::equal(ending_string.rbegin(), ending_string.rend(), complete_string.rbegin());
}
