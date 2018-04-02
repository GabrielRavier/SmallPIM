#ifndef DATEBOOK_H
#define DATEBOOK_H

#include <set>
#include <map>

#include "Appointment.h"

class DateBook
{
    // Data structure abbreviations
    typedef std::multiset<Appointment>              apptByTime_t;
    typedef std::map<int, apptByTime_t::iterator>   apptByID_t;
public:
    DateBook();
    ~DateBook();

    // Exception classes
    class AppointmentNotFound { };
    class DuplicateID { };

    int insertAppointment(const Appointment& appt, int recordID = 0) throw (DuplicateID);
    void eraseAppointment(int recordID) throw (AppointmentNotFound);
    void replaceAppointment (const Appointment& appt, int recordID = 0) throw (AppointmentNotFound);
    const Appointment& getAppointment(int recordID) const throw (AppointmentNotFound);

    /// Iterator to traverse Appointment records
    typedef apptByTime_t::const_iterator const_iterator;

    // Functions to traverse all Appointment records
    const_iterator begin() const
    {
        return m_appointments.begin();
    }

    const_iterator end() const
    {
        return m_appointments.end();
    }

    /// Find first Appointment with start time greater-than-or-equal to specified time
    const_iterator findAppointmentAtTime(const DateTime& dt) const;

    /** Find next Appointment in which any field contains the specified string. Indicate starting point for search with start
        parameter. */
    const_iterator findNextContains(const std::string& searchStr, const_iterator start) const;

    /// Return iterator to specified records ID.
    const_iterator findRecordID(int recordID) const throw (AppointmentNotFound);
private:
    // Disable copying
    DateBook(const DateBook&);
    DateBook& operator=(const DateBook&);

    /// Next free ID
    static int m_nextID;

    apptByTime_t    m_appointments;
    apptByID_t      m_apptByID;

    /// Get the index of the record with the specified ID.
    apptByTime_t::iterator getByID(int recordID) throw (AppointmentNotFound);

    /// Get the index of the record with the specified ID.
    apptByTime_t::const_iterator getByID(int recordID) const throw (AppointmentNotFound);
};

#endif // DATEBOOK_H
