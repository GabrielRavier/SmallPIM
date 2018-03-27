#include "DateTime.h"

#include <iomanip>
#include <sstream>

using std::tm;
using std::setfill;
using std::right;
using std::setw;
using std::ios;
using std::dec;
using std::stringstream;
using std::ostringstream;
using std::istringstream;
using std::string;

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

DateTime DateTime::now()
{
    DateTime ret;
    ret.m_DateTime = time(0);
    return ret;
}

std::ostream& operator<<(std::ostream& os, const DateTime& dt)
{
    ostringstream tempstring;

    tm DateTm = *localtime(&dt.m_DateTime);

    tempstring << (DateTm.tm_mon + 1) << '/' << DateTm.tm_mday << '/' << (DateTm.tm_year + 1900) << ' ';

    int hour = DateTm.tm_hour % 12;
    const char* ampm = (DateTm.tm_hour < 12 ? "am" : "pm");

    if (!hour)
        hour = 12;

    tempstring << setfill(' ') << right << setw(2) << hour << ':' << setfill('0') << right << setw(2) << DateTm.tm_min << ampm;

    return os << tempstring.str();
}

std::istream& operator>>(std::istream& is, DateTime& dt)
{
    // First, read the date part
    string date;
    is >> date;
    if (is.fail())
        return is;  // I/O error

    char slash1, slash2, colon;
    int mon, day, year, hour, min;
    char ampm[3];

    // Unpack the date using a string stream
    istringstream tempdate(date);
    tempdate >> mon >> slash1 >> day >> slash2 >> year;

    // Check for error in date
    if (tempdate.fail() || slash1 != '/' || slash2 != '/' || mon < 1 || 12 < mon || day < 1 || 31 < day)
    {
        // Format error, set fail bit and return
        is.clear(ios::failbit);
        return is;
    }

    // Now read the time part
    string time;
    is >> time;
    if (is.fail())
        return is;  // I/O error

    // Unpack the time part using the string stream. Do not read am/pm indicator yet
    tempdate.clear();
    tempdate.str(time);
    tempdate >> hour >> colon >> min;

    // Check for error in time
    if (tempdate.fail() || hour < 0 || 23 < hour || min < 0 || 59 < min)
    {
        // Format error, set fail bit and return
        is.clear(ios::failbit);
        return is;
    }

    // Check for am/pm indicator
    tempdate >> setw(3) >> ampm;
    bool useAsPm = ! tempdate.fail();

    // Convert hour from 12-hour to 24-hour format (0-23)
    if (useAsPm)
    {
        if (hour == 12)
            hour = 0;
        if (ampm[0] == 'p' || ampm[0] == 'P')
            hour += 12;
    }

    dt = DateTime(year, mon, day, hour, min);

    return is;
}
