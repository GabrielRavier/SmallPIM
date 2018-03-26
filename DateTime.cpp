#include "DateTime.h"

#include <iomanip>

using std::tm;
using std::setfill;
using std::right;
using std::setw;

DateTime::DateTime(int year, int month, int day, int hour, int min)
{
    set(year, month, day, hour, min);
}

void DateTime::get(int& year, int& month, int& day, int& hour, int& min) const
{
    tm* mytm = localtime(&m_DateTime);
    year    = mytm->tm_year + 1900;
    month   = mytm->tm_mon + 1;
    day     = mytm->tm_mday;
    hour    = mytm->tm_hour;
    min     = mytm->tm_min;
}

DateTime& DateTime::set(int year, int month, int day, int hour, int min)
{
    /* Years from 0-49 are assumed to mean 2000-2049. Years 1900 or over are assumed to be 4-digit years.
       Other years are assumed to mean 1950-1999. */
    if (year < 50)
        year += 2000;
    else if (year < 1900)
        year += 1900;

    tm mytm;
    mytm.tm_year    = year - 1900;
    mytm.tm_mon     = month - 1; // Zero based
    mytm.tm_mday    = day;
    mytm.tm_hour    = hour;
    mytm.tm_min     = min;
    mytm.tm_sec     = 0;
    mytm.tm_isdst   = -1;

    m_DateTime = mktime(&mytm);

    return *this;
}
