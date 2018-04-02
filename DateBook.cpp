#include "DateBook.h"
#include <algorithm>

using std::unary_function;
using std::string;
using std::find_if;

int DateBook::m_nextID = 1;

DateBook::DateBook()
{

}

DateBook::~DateBook()
{

}

int DateBook::insertAppointment(const Appointment& appt, int recordID) throw (DuplicateID)
{
    if (!recordID)
        // If recordID is not specified, create a new record ID
        recordID = m_nextID++;
    else if (recordID >= m_nextID)
        // Make sure nextID is always higher than any known ID
        m_nextID = recordID + 1;
    else if (m_apptByID.count(recordID))
        // recordID is already in map
        throw DuplicateID();

    // Assign recordID to copy of Appointment
    Appointment apptCopy(appt);
    apptCopy.recordID(recordID);

    // Insert record into set
    apptByTime_t::iterator i = m_appointments.insert(apptCopy);

    // Insert Appointment iterator into ID-based map
    // m_apptByID.insert(make_pair(recordID, i));
    m_apptByID[recordID] = i;

    return recordID;
}

DateBook::apptByTime_t::iterator DateBook::getByID(int recordID) throw (AppointmentNotFound)
{
    // Find record by ID
    apptByID_t::iterator IDIter = m_apptByID.find(recordID);
    if (IDIter == m_apptByID.end())
        throw AppointmentNotFound();

    return IDIter->second;
}

DateBook::apptByTime_t::const_iterator DateBook::getByID(int recordID) const throw (AppointmentNotFound)
{
    // Find record by ID
    apptByID_t::const_iterator IDIter = m_apptByID.find(recordID);
    if (IDIter == m_apptByID.end())
        throw AppointmentNotFound();

    return IDIter->second;
}

void DateBook::eraseAppointment(int recordID) throw (AppointmentNotFound)
{
    apptByTime_t::iterator i = getByID(recordID);

    // Remove entry from both containers
    m_appointments.erase(i);
    m_apptByID.erase(recordID);
}

void DateBook::replaceAppointment(const Appointment& appt, int recordID) throw (AppointmentNotFound)
{
    if (!recordID)
        recordID = appt.recordID();

    eraseAppointment(recordID);
    insertAppointment(appt, recordID);
}

const Appointment& DateBook::getAppointment(int recordID) const throw (AppointmentNotFound)
{
    return *getByID(recordID);
}

// Find first appointment with start time greater-than-or-equal to specified time
DateBook::const_iterator DateBook::findAppointmentAtTime(const DateTime& dt) const
{
    Appointment searchAppt;
    searchAppt.startTime(dt);

    return m_appointments.lower_bound(searchAppt);
}

// Function object class to search for a string within an Appointment
class AppointmentContainsStr : public unary_function<Appointment, bool>
{
public:
    AppointmentContainsStr(const string& str) : m_str(str)
    {

    }

    /// Return true if any Appointment field contains m_str
    bool operator()(const Appointment& a)
    {
        return (a.description().find(m_str) != string::npos);
    }

private:
    string m_str;
};

/* Find next Appointment in which any field contains the specified string.
   Indicate starting point for search with start parameter.
*/
DateBook::const_iterator DateBook::findNextContains(const std::string& searchStr, const_iterator start) const
{
    return find_if(start, m_appointments.end(), AppointmentContainsStr(searchStr));
}

// Return iterator to specified record's ID.
DateBook::const_iterator DateBook::findRecordID(int recordID) const throw (AppointmentNotFound)
{
    return getByID(recordID);
}
