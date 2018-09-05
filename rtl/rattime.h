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

#pragma once

#include <string>
#include <cstdint>
#include <ctime>

/*
	i.e. a diff in msecs between January 0, 1900 and 1970-01-01T00:00:00.000
	In Delphi/FPC: EncodeDate(1970, 1, 1)*MSecsPerDay aka UnixDateDelta*MSecsPerday
*/
constexpr int64_t diffFrom1970To1900 = 2209161600000;
// seconds per day
constexpr double MSecsPerDay = 24.0*60.0*60.0*1000.0;
// reciproc value of seconds per day for faster calculations
constexpr double InvMSecsPerDay = 1.0 / MSecsPerDay;

// gets current time offset from UTC
int Get_UTC_Offset();

// converts given unix timestamp to rat time
double Unix_Time_To_Rat_Time(const time_t qdt);
// converts given rat time to unix timestamp
time_t Rat_Time_To_Unix_Time(const double rt);

// fmt is the string formatting used by C++ standard library
std::string Rat_Time_To_Local_Time_Str(const double rt, const char *fmt);
// fmt is the string formatting used by C++ standard library
std::wstring Rat_Time_To_Local_Time_WStr(const double rt, const wchar_t *fmt);

// converts string representation of local time (str) to rat time; fmt is the string formatting used by C++ standard library
double Local_Time_WStr_To_Rat_Time(const std::wstring& str, const wchar_t* fmt);