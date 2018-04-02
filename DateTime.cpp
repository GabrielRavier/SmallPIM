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

void DateTime::getDate(int& year, int& month, int& day) const
{
    int hour, min;
    get(year, month, day, hour, min);
}

void DateTime::getTime(int& hour, int& min) const
{
    int year, month, day;
    get(year, month, day, hour, min);
}

// Return the day of the week (range 0 = Sunday to 6 = Saturday)
int DateTime::dayOfWeek() const
{
    return localtime(&m_DateTime)->tm_wday;
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

DateTime& DateTime::setDate(int year, int month, int day)
{
    int hour, min;
    getTime(hour, min);
    set(year, month, day, hour, min);

    return *this;
}

DateTime& DateTime::setTime(int hour, int min)
{
    int year, month, day;
    getDate(year, month, day);
    set(year, month, day, hour, min);

    return *this;
}

std::string DateTime::dateStr() const
{
    char buf[20];
    strftime(buf, 20, "%x", localtime(&m_DateTime));
    return buf;
}

std::string DateTime::timeStr() const
{
    char buf[20];
    strftime(buf, sizeof(buf), "%I:%M%p", localtime(&m_DateTime));
    return buf;
}


// Name of day of the week
std::string DateTime::wdayName() const
{
    char buf[30];
    strftime(buf, sizeof(buf), "%A", localtime(&m_DateTime));
    return buf;
}

// Name of month of the year
std::string DateTime::monthName() const
{
    char buf[30];
    strftime(buf, sizeof(buf), "%B", localtime(&m_DateTime));
    return buf;
}

bool DateTime::dateStr(const std::string& s)
{
    char slash1, slash2;
    int mon, day, year;

    // Unpack the date part using a string stream
    istringstream tempstream(s);
    tempstream >> mon >> slash1 >> day >> slash2 >> year;

    // Check for error in date
    if (tempstream.fail() || slash1 != '/' || slash2 != '/' || mon < 1 || 12 < mon || day < 1 || 31 < day)
        return false;

    setDate(year, mon, day);
    return true;
}

bool DateTime::timeStr(const std::string& s)
{
    char colon;
    int hour, min;
    char ampm[3];

    istringstream tempstream(s);
    tempstream >> hour >> colon >> min;

    // Check for error in time
    if (tempstream.fail() || hour < 0 || 23 < hour || min < 0 || 59 < min)
        return false;

    // Check for am/pm indicator
    tempstream >> setw(3) >> ampm;
    bool useAmPm = ! tempstream.fail();

    // Convert hour from 12-hour to 24-hour format (0-23)
    if (useAmPm)
    {
        if (hour == 12)
            hour = 0;
        if (ampm[0] == 'p' || ampm[0] == 'P')
            hour += 12;
    }

    setTime(hour, min);
    return true;
}

// Return this day at midnight
DateTime DateTime::startOfDay() const
{
    tm DtTm = *localtime(&m_DateTime);
    DtTm.tm_hour = 0;
    DtTm.tm_min = 0;
    DtTm.tm_sec = 0;
    DtTm.tm_isdst = -1;

    DateTime result;
    result.m_DateTime = mktime(&DtTm);
    return result;
}

// Return most recent Sunday at midnight
DateTime DateTime::startOfWeek() const
{
    tm DtTm = *localtime(&m_DateTime);
    DtTm.tm_mday -= DtTm.tm_wday;   // Substract current day-of week
    DtTm.tm_hour = 0;
    DtTm.tm_min = 0;
    DtTm.tm_sec = 0;
    DtTm.tm_isdst = -1;

    DateTime result;
    result.m_DateTime = mktime(&DtTm);
    return result;
}

// Return first day of this month
DateTime DateTime::startOfMonth() const
{
    tm DtTm = *localtime(&m_DateTime);
    DtTm.tm_mday = 1;
    DtTm.tm_hour = 0;
    DtTm.tm_min = 0;
    DtTm.tm_sec = 0;
    DtTm.tm_isdst = -1;

    DateTime result;
    result.m_DateTime = mktime(&DtTm);
    return result;
}

// Add a certain number of days to the date
DateTime DateTime::addDay(int days) const
{
    tm DtTm = *localtime(&m_DateTime);
    DtTm.tm_mday += days;
    DtTm.tm_isdst = -1;

    DateTime result;
    result.m_DateTime = mktime(&DtTm);
    return result;
}

// Get the current date and time
DateTime DateTime::now()
{
    DateTime ret;
    ret.m_DateTime = time(0);
    return ret;
}

std::ostream& operator<<(std::ostream& os, const DateTime& dt)
{
    return os << (dt.dateStr() + " " + dt.timeStr());
}

std::istream& operator>>(std::istream& is, DateTime& dt)
{
    DateTime result;

    // First, read the date part
    string date;
    is >> date;
    if (is.fail())
        return is;  // I/O error

    if (!result.dateStr(date))
    {
        is.clear(ios::failbit);
        return is;  // Format error
    }

    string time;
    is >> time;
    if (is.fail())
        return is;  // I/O error

    if (!result.timeStr(time))
    {
        is.clear(ios::failbit);
        return is;  // Format error
    }

    dt = result;
    return is;
}
