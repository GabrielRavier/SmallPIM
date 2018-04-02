#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>
#include "DateTime.h"

class Appointment
{
public:
    Appointment() : m_recordID(0)
    {

    }

    /// \return Appointment's recordID
    int recordID() const
    {
        return m_recordID;
    }

    /** Change recordID of appointment
        \param i : new record ID */
    void recordID(int i)
    {
        m_recordID = i;
    }

    /// \return Appointment's start time
    DateTime startTime() const
    {
        return m_startTime;
    }

    /** Change start time of appointment
        \param dt : new start time
    */
    void startTime(const DateTime& dt);

    /// \return Appointment's end time
    DateTime endTime() const
    {
        return m_endTime;
    }

    /** Change end time of appointment
        \param dt : new end time
    */
    void endTime(const DateTime& dt);

    /// \return Appointment's description
    std::string description() const
    {
        return m_description;
    }

    /** Change appointment's description
        \param s : new description
    */
    void description(const std::string& s);

private:
    /// Record ID of the appointment
    int         m_recordID;
    /// Start time of the appointment
    DateTime    m_startTime;
    /// End time of the appointment
    DateTime    m_endTime;
    /// Description of the appointment
    std::string m_description;
};

inline bool operator< (const Appointment& a1, const Appointment& a2)
{
    return a1.startTime() < a2.startTime();
}

#endif // APPOINTMENT_H
