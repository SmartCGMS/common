#include "rattime.h"
#include "winapi_mapping.h"

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