/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
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

	return Unix_Time_To_Rat_Time(ltim);
}