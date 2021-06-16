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

#include "rattime.h"
#include "../iface/DeviceIface.h"
#include "../utils/winapi_mapping.h"
#include "../utils/string_utils.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>

#undef max

int Get_UTC_Offset()
{
	time_t gmt, rawtime = time(nullptr);
	struct tm ptm {}; //zero initalize all possibly non-set elements like daylight saving

	gmtime_s(&ptm, &rawtime);

	ptm.tm_isdst = -1;
	gmt = mktime(&ptm);

	return static_cast<int>(difftime(rawtime, gmt));
}

double Unix_Time_To_Rat_Time(const time_t qdt)
{
	int64_t diff = static_cast<int64_t>(qdt)*1000 + diffFrom1970To1900;
	return static_cast<double>(diff) * InvMSecsPerDay;
}

time_t Rat_Time_To_Unix_Time(const double rt)
{
	double diff = rt * MSecsPerDay;
	int64_t msecs = (static_cast<int64_t>(ceil(diff))) - diffFrom1970To1900;	

	return static_cast<time_t>(msecs / 1000);
}

std::string Rat_Time_To_Local_Time_Str(const double rt, const char *fmt) {
	time_t ltim = Rat_Time_To_Unix_Time(rt);
	struct tm ptm {}; //zero initalize all possibly non-set elements like daylight saving
	localtime_s(&ptm, &ltim);

	std::ostringstream os;
	os << std::put_time(&ptm, fmt);
	
	return os.str();
}

std::wstring Rat_Time_To_Local_Time_WStr(const double rt, const wchar_t *fmt) {
	if (rt == 0.0) return std::wstring{};

	time_t ltim = Rat_Time_To_Unix_Time(rt);

	struct tm ptm {}; //zero initalize all possibly non-set elements like daylight saving
	localtime_s(&ptm, &ltim);

	std::wostringstream os;
	os << std::put_time(&ptm, fmt);

	return os.str();
}

double Local_Time_WStr_To_Rat_Time(const std::wstring& str, const wchar_t* fmt) noexcept {

	struct tm ptm {};	//zero initialized, otherwise a ranomd value may randomly trigger daylight saving offset on/off
	std::wistringstream ss(str);
	ss >> std::get_time(&ptm, fmt);	

	time_t ltim = mktime(&ptm);

	return ltim != -1 ? Unix_Time_To_Rat_Time(ltim) : std::numeric_limits<double>::quiet_NaN();
}

std::wstring Rat_Time_To_Default_WStr(double rattime) {
	if (std::isnan(rattime))
		return L"NaN";

	auto calc_fraction = [&](const double factor) {
		double intpart;
		rattime *= factor;
		rattime = std::modf(rattime, &intpart);
		return intpart;
	};


	auto add_fraction = [&](const double intpart, const double factor) {
		if (factor == 1.0) {	//days
			return intpart != 0.0 ? std::to_wstring(static_cast<int>(intpart)) + L' ' : L"";
		}
		else {
			std::wstring result = std::to_wstring(static_cast<int>(intpart));
			while (result.size() < 2)
				result = L'0' + result;
			return result;
		}
	};


	//handle the sign
	std::wstring result{ rattime < 0.0 ? L"-" : L"" };
	rattime = std::fabs(rattime);

	//decompose to individual parts
	double days = calc_fraction(1.0);
	double hours = calc_fraction(24.0);
	double minutes = calc_fraction(60.0);
	double seconds = calc_fraction(60.0);

	//perform round-up to seconds
	if (std::round(rattime) > 0.0) {
		seconds++;

		if (seconds >= 60.0) {
			minutes++;
			seconds = 0.0;

			if (minutes >= 60.0) {
				hours++;
				minutes = 0.0;

				if (hours >= 24.0) {
					days++;
				}
			}
		}
	}

	//and compose the string
	result += add_fraction(days, 1.0);
	result += add_fraction(hours, 24.0) + L':';
	result += add_fraction(minutes, 60.0) + L':';
	result += add_fraction(seconds, 60.0);

	return result;
}


bool is_digit(const char ch) {
	switch (ch) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9': return true;
		default: return false;
	}
}

bool is_digit(const wchar_t ch) {
	switch (ch) {
		case L'0': case L'1': case L'2': case L'3': case L'4':
		case L'5': case L'6': case L'7': case L'8': case L'9': return true;
		default: return false;
	}
}

size_t str_len(const char* str) { return strlen(str); }
size_t str_len(const wchar_t* str) { return wcslen(str); }

template <typename T>
struct TTime_Chars {
	static constexpr T minus = '-';
	static constexpr T colon = ':';
	static constexpr T dot = '-';
	static constexpr T space = ' ';
};

template <>
struct TTime_Chars<wchar_t>{
	static constexpr wchar_t minus = L'-';
	static constexpr wchar_t colon = L':';
	static constexpr wchar_t dot = L'-';
	static constexpr wchar_t space = L' ';
};

template <typename C>
double Convert_Str_To_Rat_Time(const C* input, bool& converted_ok) {
	converted_ok = false;	//assume an error (and simplify the exit code)

	double days = 0.0, hours = 0.0, minutes = 0.0, seconds = 0.0;

	double plus_minus_sign = 1.0;
	int plus_minus_pos = 0;	//must be signed int!
	
	if (!input || (*input == 0))  return std::numeric_limits<double>::quiet_NaN();	
		
	if (input[0] == TTime_Chars<C>::minus) {
		plus_minus_sign = -1.0;
		plus_minus_pos = 1;
	}


	int pos, last_pos = static_cast<int>(str_len(input));
	

	auto fetch_number = [&](const C sep, const C decimal, double& result, const double result_max) {
		pos = last_pos - 1;

		while (pos >= plus_minus_pos) {
			const C ch = input[pos];
			if (ch == sep) break;
			if (!is_digit(ch) && (ch != decimal)) return false;	

			pos--;
		}

		pos++;
		std::basic_string<C, std::char_traits<C>, std::allocator<C>> substring(&input[pos], static_cast<size_t>(last_pos) - static_cast<size_t>(pos));	//must be a string to add the terminating zero
		bool ok;
		result = str_2_dbl(substring.c_str(), ok);			

		if ((!ok) || (result >= result_max)) return false;

		last_pos = pos - 1;
		return true;
	};


	//search for seconds, minutes, hours and days

	if (!fetch_number(TTime_Chars<C>::colon, TTime_Chars<C>::dot, seconds, 60.0)) return false;

	if (last_pos > plus_minus_pos) {
		if (!fetch_number(TTime_Chars<C>::colon, 0, minutes, 60.0)) return false;

		if (last_pos > plus_minus_pos) {
			if (!fetch_number(TTime_Chars<C>::space, 0, hours, 24.0)) return false;

			if (last_pos > plus_minus_pos)
				if (!fetch_number(TTime_Chars<C>::minus, 0, days, std::numeric_limits<double>::max()))
					return std::numeric_limits<double>::quiet_NaN();				
		}
	}

	converted_ok = true;
	return plus_minus_sign * (days + scgms::One_Hour * hours + scgms::One_Minute * minutes + scgms::One_Second * seconds);

	return true;
}

double Default_Str_To_Rat_Time(const std::wstring& input, bool& converted_ok) {
	return Default_Str_To_Rat_Time(input.c_str(), converted_ok);
}

double Default_Str_To_Rat_Time(const wchar_t* input, bool& converted_ok) {
	return Convert_Str_To_Rat_Time(input, converted_ok);
}

double Default_Str_To_Rat_Time(const char* input, bool& converted_ok) {
	return Convert_Str_To_Rat_Time(input, converted_ok);
}