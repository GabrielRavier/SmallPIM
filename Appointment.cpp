#include "Appointment.h"

void Appointment::startTime(const DateTime& dt)
{
    m_startTime = dt;
}

void Appointment::endTime(const DateTime& dt)
{
    m_endTime = dt;
}

void Appointment::description(const std::string& s)
{
    m_description = s;
}
