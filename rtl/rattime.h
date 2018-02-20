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
int GetUTCOffset();

// converts given unix timestamp to rat time
double UnixTime2RatTime(const time_t qdt);
// converts given rat time to unix timestamp
time_t RatTime2UnixTime(const double rt);

// fmt is the string formatting used by C++ standard library
void RatTime2LocalTimeStr(const double rt, const char *fmt, std::string &dst);
// fmt is the string formatting used by C++ standard library
void RatTime2LocalTimeWStr(const double rt, const wchar_t *fmt, std::wstring &dst);
