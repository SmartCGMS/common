#include "rattime.h"
#include "winapi_mapping.h"

#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>

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
	int64_t diff = qdt*1000 + diffFrom1970To1900;
	return static_cast<double>(diff) * InvMSecsPerDay;
}

time_t Rat_Time_To_Unix_Time(const double rt)
{
	double diff = rt*MSecsPerDay;
	time_t msecs = ((time_t)ceil(diff)) - diffFrom1970To1900;

	return msecs / 1000;
}

void Rat_Time_To_Local_Time_Str(const double rt, const char *fmt, std::string &dst)
{
	time_t ltim = Rat_Time_To_Unix_Time(rt);
	struct tm ptm;
	localtime_s(&ptm, &ltim);

	std::ostringstream os;
	os << std::put_time(&ptm, fmt);
	
	dst = os.str();
}

void Rat_Time_To_Local_Time_WStr(const double rt, const wchar_t *fmt, std::wstring &dst)
{
	time_t ltim = Rat_Time_To_Unix_Time(rt);
	struct tm ptm;
	localtime_s(&ptm, &ltim);

	std::wostringstream os;
	os << std::put_time(&ptm, fmt);

	dst = os.str();
}
