#include "AppointmentEditor.h"

#include <iostream>
#include <sstream>

using std::string;
using std::cout;
using std::endl;

// Start with an empty Appointment object
AppointmentEditor::AppointmentEditor()
{

}

// Edit an existing Appointment object
AppointmentEditor::AppointmentEditor(const Appointment& appt) : m_appt(appt)
{

}

// Main loop returns true if appointment was successfully edited, false if edit was aborted.
bool AppointmentEditor::edit()
{
    // Unpack the appointment
    DateTime startTime(m_appt.startTime());
    DateTime endTime(m_appt.endTime());
    string description(m_appt.description());

    editDate("Date", startTime) &&
    editTime("Start Time", startTime) &&
    editTime("End Time", endTime) &&
    editMultiLine("Description", description);

    if (status() == canceled)
        return false;

    // Make sure that end time is after start time and that they are less than 24-hours apart.
    int year, month, day;
    startTime.getDate(year, month, day);
    endTime.setDate(year, month, day);

    // If endTime is after start time, advance end time to next day.
    if (endTime < startTime)
        endTime = endTime.addDay();

    // Commit changes
    m_appt.startTime(startTime);
    m_appt.endTime(endTime);
    m_appt.description(description);

    return status() != canceled;
}

bool AppointmentEditor::editDate(const std::string& prompt, DateTime& dt)
{
    string dateStr = dt.dateStr();

    for (;;)
    {
        if (!editSingleLine(prompt, dateStr))
            return false;

        // Check for special string "today"
        if (!dateStr.empty() && (dateStr[0] == 'T' || dateStr[0] == 't'))
        {
            dt = DateTime::now();
            return true;
        }

        // Convert back from string rep to DateTime
        if (!dt.dateStr(dateStr))
            cout << "Invalid date. Try again" << endl;
        else
            return true;
    }
}

bool AppointmentEditor::editTime(const std::string& prompt, DateTime& dt)
{
    string timeStr = dt.timeStr();

    for (;;)
    {
        if (!editSingleLine(prompt, timeStr))
            return false;

        // Convert back from string rep to DateTime
        if (!dt.timeStr(timeStr))
            cout << "Invalid time. Try again" << endl;
        else
            return true;
    }
}
