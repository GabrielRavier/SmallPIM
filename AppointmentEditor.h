#ifndef APPOINTMENTEDITOR_H
#define APPOINTMENTEDITOR_H

#include "Editor.h"

#include <string>

#include "Appointment.h"

/// Class for editing an Appointment object
class AppointmentEditor : public Editor
{
public:
    /// Start with an empty Appointment object
    AppointmentEditor();

    /// Edit an existing Appointment object
    AppointmentEditor(const Appointment& appt);

    // Use compiler-generated destructor
    // ~AppointmentEditor

    /// Main loop returns true if appointment was successfully edited, false if edit was aborted.
    bool edit();

    /// This accessor is used to retrieve the modified appointment.
    Appointment appt() const
    {
        return m_appt;
    }

    /// This accessor is used to set the Appointment object to edit:
    void appt(const Appointment& appt)
    {
        m_appt = appt;
    }

private:
    // Disable copying
    AppointmentEditor(const AppointmentEditor&);
    const AppointmentEditor& operator=(const AppointmentEditor&);

    /// Appointment which is edited
    Appointment m_appt;

protected:
    // Protected functions
    bool editDate(const std::string& prompt, DateTime& dt);
    bool editTime(const std::string& prompt, DateTime& dt);
};

#endif // APPOINTMENTEDITOR_H
