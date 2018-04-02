#ifndef DATETIME_H
#define DATETIME_H

#include <iostream>
#include <string>
#include <ctime>

/// Class for containing a date and time
class DateTime
{
public:
    DateTime() : m_DateTime(0)
    {

    }
    DateTime(int year, int month, int day, int hour, int min);

    // Use compiler-generated copy constructor, assignment, destructor

    /// Read accessor
    void get(int& year, int& month, int& day, int& hour, int& min) const;
    /// Read accessor for date
    void getDate(int& year, int& month, int& day) const;
    /// Read accessor for time of day
    void getTime(int& hour, int& min) const;

    /// Return day of the week (range 0 = Sunday to 6 = Saturday)
    int dayOfWeek() const;

    /// Write accessor
    DateTime& set(int year, int month, int day, int hour, int min);
    /// Write accessor for date
    DateTime& setDate(int year, int month, int day);
    /// Write accessor for time of day
    DateTime& setTime(int hour, int min);

    /// Return this day at midnight
    DateTime startOfDay() const;

    /// Return most recent Monday at midnight
    DateTime startOfWeek() const;

    /// Return first day of this month
    DateTime startOfMonth() const;

    /// Add a certain number of days to the date
    DateTime addDay(int days = 1) const;

    /// Return date in string form
    std::string dateStr() const;

    /// Return time in string form
    std::string timeStr() const;

    /// Return name of day of the week
    std::string wdayName() const;

    /// Return name of month of the year
    std::string monthName() const;

    /// Parse string and set the date
    bool dateStr(const std::string& s);

    /// Parse string and set the time
    bool timeStr(const std::string& s);

    /// Get the current date and time
    static DateTime now();

    friend bool operator==(const DateTime& dt1, const DateTime& dt2)
    {
        return dt1.m_DateTime == dt2.m_DateTime;
    }

    friend bool operator<(const DateTime& dt1, const DateTime& dt2)
    {
        return difftime(dt1.m_DateTime, dt2.m_DateTime);
    }

    friend std::ostream& operator<<(std::ostream& os, const DateTime& dt);

    friend std::istream& operator>>(std::istream& is, DateTime& dt);

    private:
    /// Date and time
    time_t m_DateTime;
};

#endif // DATETIME_H
