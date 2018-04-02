#ifndef APPOINTMENTDISPLAYLIST_H
#define APPOINTMENTDISPLAYLIST_H

#include "DisplayList.h"
#include "DateTime.h"

#include <string>
#include <vector>

class DateBook;

/// Specialized DisplayList for Appointment records.
class AppointmentDisplayList : public DisplayList
{
public:
    /// Construct with a reference to the address book
    AppointmentDisplayList(DateBook& dateBook);

    DateTime currDate() const
    {
        return m_currDate;
    }
    void currDate(DateTime dt);

    /// List appointments for the day containing the specified DateTime
    void listDay(DateTime dt);

    /// List appointments for the week containing the specified DateTime
    void listWeek(DateTime dt);

    /// List all records that contain the specified string
    void listContainsString(const std::string&);

protected:
    /// Display the specified appointment record in one-line format. Implements pure virtual base-class function.
    virtual void displayRecord(int recordID);

    /// Override base-class function to retrieve more records.
    virtual bool fetchMore(int startID, int numRecords, std::vector<int>& result);

private:
    DateBook& m_dateBook;

    /// Listing modes
    enum listMode
    {
        dayMode,
        weekMode,
        stringMode
    };
    listMode m_mode;

    /// DateTime used for dayMode and weekMode
    DateTime m_currDate;

    /// String to use for stringMode
    std::string m_containsString;
};

#endif // APPOINTMENTDISPLAYLIST_H
