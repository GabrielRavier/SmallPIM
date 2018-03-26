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

    /// Write accessor
    DateTime& set(int year, int month, int day, int hour, int min);

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

    friend std::istream& operator>>(std::istream& is, const DateTime& dt);

    private:
    /// Date and time
    time_t m_DateTime;
};

#endif // DATETIME_H
