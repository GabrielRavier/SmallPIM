#include "AppointmentDisplayList.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "Appointment.h"
#include "DateBook.h"
#include "DateTime.h"

using std::cout;
using std::min;

// Construct with a reference to the address book
AppointmentDisplayList::AppointmentDisplayList(DateBook& dateBook) : m_dateBook(dateBook), m_mode(dayMode)
    , m_currDate(DateTime::now())
{

}

// Display the specified appointment record in one-line format.
void AppointmentDisplayList::displayRecord(int recordID)
{
    if (recordID < 0)
    {
        // A negative recordID means a special date marker in the range -1 (Sunday) to -7 (Saturday). Print the date marker.
        DateTime startOfWeek = m_currDate.addDay(-m_currDate.dayOfWeek());
        DateTime marker = startOfWeek.addDay(-recordID - 1);
        cout << marker.wdayName() << ' ' << marker.dateStr();
        return;
    }

    Appointment record = m_dateBook.getAppointment(recordID);

    // Print prefix for entry.
    switch (m_mode)
    {
    case dayMode:
        break;
    case weekMode:
        // For week mode, indent each entry
        cout << "    ";
        break;
    case stringMode:
        // For string search view, put date on every line
        cout << record.startTime().dateStr() << " ";
        break;
    }

    // Format start and end time
    cout << record.startTime().timeStr() << " - " << record.endTime().timeStr();

    // Output 45 characters of the description or up to the first newline, whichever is shorter.
    int outlen = min(45, (int)record.description().find('\n'));
    cout << " " << record.description().substr(0, outlen);
}

// Fetch more records from DateBook
bool AppointmentDisplayList::fetchMore(int startID, int numRecords, std::vector<int>& result)
{
    // Remove old contents of result
    result.clear();

    if (!numRecords)
        return false;

    bool forwards = true;
    if (numRecords < 0)
    {
        forwards = false;
        numRecords = -numRecords;
    }

    // Check for empty list
    if (m_dateBook.begin() == m_dateBook.end())
        return true;

    // Declare an iterator
    DateBook::const_iterator iter;

    /* Get iterator to record specified by startID. When fetching forward, increment iterator past matching record to avoid
       a duplicate insertion into the display list. */
    if (!startID)
        iter = (forwards ? m_dateBook.begin() : m_dateBook.end());
    else
    {
        iter = m_dateBook.findRecordID(startID);
        if (forwards)
            ++iter;
    }

    if (m_mode != stringMode)
    {
        // "List week" or "List day" mode
        DateTime firstDate, lastDate;
        if (m_mode == dayMode)
        {
            // Set time range from midnight at start of day to midnight at end of day
            firstDate = m_currDate.startOfDay();
            lastDate = firstDate.addDay();
        }
        else
        {
            // Set time range from midnight on prior Sunday to midnight on following Sunday
            firstDate = m_currDate.startOfDay();
            firstDate = firstDate.addDay(-firstDate.dayOfWeek());
            lastDate = firstDate.addDay(7);
        }

        /* We will always fill the cache completely in one call, so we force an empty cache to start with. We should never
           actually reach this point unless the cache is already empty, but just in case ...*/

        if (startID)
            reset();

        DateBook::const_iterator startIter = m_dateBook.findAppointmentAtTime(firstDate);
        DateBook::const_iterator endIter = m_dateBook.findAppointmentAtTime(lastDate);

        /* Retrieve records starting at iter (doesn't matter whether we are searching forwards or backwards because we are always
           getting a complete set */
        DateTime prevDate;
        for (iter = startIter; iter != endIter; ++iter)
        {
            using namespace std::rel_ops; // To get != operator
            if (m_mode == weekMode && iter->startTime().startOfDay() != prevDate)
            {
                // Start of a new day. Push a negative number for the day of the week in the range -1 (Sunday) to -7 (Saturday)
                result.push_back(-iter->startTime().dayOfWeek() - 1);
                prevDate = iter->startTime().startOfDay();
            }
            result.push_back(iter->recordID());
        }

        // Return true because we reached end of the list
        return true;
    }
    else
    {
        // "Contains string" mode

        if (forwards)
        {
            // Retrieve records AFTER startID

            // Find matching record starting at iter
            iter = m_dateBook.findNextContains(m_containsString, iter);
            while (iter != m_dateBook.end() && numRecords-- > 0)
            {
                result.push_back(iter->recordID());

                // Find matching record
                iter = m_dateBook.findNextContains(m_containsString, ++iter);
            }

            // Return true if we reached the end
            return iter == m_dateBook.end();
        }
        else
        {
            // Retrieve records BEFORE startID

            // DateBook does not have a function to search backwards. Instead, we retrieve ALL records before iter
            DateBook::const_iterator endIter = iter;
            iter = m_dateBook.findNextContains(m_containsString, m_dateBook.begin());

            while (iter != endIter)
            {
                result.push_back(iter->recordID());
                iter = m_dateBook.findNextContains(m_containsString, ++iter);
            }

            return true;    // Yes, we reached the start of the list
        }
    }
}

// Set the current date
void AppointmentDisplayList::currDate(DateTime dt)
{
    if (dt == m_currDate)
        return;

    m_currDate = dt;
    reset();

    // Next call to display() will refill cache
}

// List appointments for the day containing the specified DateTime
void AppointmentDisplayList::listDay(DateTime dt)
{
    if (m_mode == dayMode && m_currDate == dt)
        return;

    m_mode = dayMode;
    currDate(dt);
}

// List appointments for the week containing the specified DateTime
void AppointmentDisplayList::listWeek(DateTime dt)
{
    if (m_mode == weekMode && m_currDate == dt)
        return;

    m_mode = weekMode;
    currDate(dt);
}

// List all records that contain the specified string
void AppointmentDisplayList::listContainsString(const std::string& s)
{
    if (m_mode == stringMode && m_containsString == s)
        return;

    m_containsString = s;
    m_mode = stringMode;
    reset();

    // Next call to display() will refill cache
}
