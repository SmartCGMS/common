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

#include "rattime.h"
#include "../utils/winapi_mapping.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>

int Get_UTC_Offset()
{
	time_t gmt, rawtime = time(nullptr);
	struct tm ptm;

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
	struct tm ptm;
	localtime_s(&ptm, &ltim);

	std::ostringstream os;
	os << std::put_time(&ptm, fmt);
	
	return os.str();
}

std::wstring Rat_Time_To_Local_Time_WStr(const double rt, const wchar_t *fmt) {
	if (rt == 0.0) return std::wstring{};

	time_t ltim = Rat_Time_To_Unix_Time(rt);

	struct tm ptm;
	localtime_s(&ptm, &ltim);

	std::wostringstream os;
	os << std::put_time(&ptm, fmt);

	return os.str();
}

double Local_Time_WStr_To_Rat_Time(const std::wstring& str, const wchar_t* fmt) {

	struct tm ptm;
	std::wistringstream ss(str);
	try
	{
		ss >> std::get_time(&ptm, fmt);
	}
	catch (...)
	{
		return std::numeric_limits<double>::quiet_NaN();
	}

	time_t ltim = mktime(&ptm);

	return ltim != -1 ? Unix_Time_To_Rat_Time(ltim) : std::numeric_limits<double>::quiet_NaN();
}
