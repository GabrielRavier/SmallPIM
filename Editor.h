#ifndef EDITOR_H
#define EDITOR_H

#include <string>

// Editor base class.
// Provides basic functionality for single- and multi-line editing.
class Editor
{
public:
    enum editStatus
    {
        /// Edit is proceeding normally.
        normal,
        /// User has indicated that editing is complete.
        finished,
        /// User has indicated that editing should be canceled.
        canceled
    };

    Editor() : m_status(normal) { }

    /**  Edit a single line of text. The prompt and initial value are passed in. This function modifies the value
         and returns true if edit status is normal or false if editing is finished or canceled. The  following
         inputs have special meaning:

             [CR]    Pressing [ENTER] will leave value unchanged.
             .       A period by itself signals that editing is complete
             !x      Signals that editing should be canceled.
    */
    bool editSingleLine(const std::string& prompt, std::string& value);

    /**  Edit a multi-line string. The prompt for all lines and an initial multi-line value are passed in. The
         edited string is stored back in value. Withing the edit session, each line is edited separately. This
         function modifies the value and returns true if edit status is normal or false if editing is finished
         or canceled. The following inputs have special meaning:

             [CR]    Pressing [ENTER] without entering any text will leave the current line unchanged and advance
                     to the next line.
             .       A period by itself signals that editing is complete.
             !x      Signals that editing should be canceled.
             !n      Editing this field is complete. Caller should advance to next field (status remains normal).
             !i      Insert a new line before the current one.
             !d      Delete the current line.
    */
    bool editMultiLine(const std::string& prompt, std::string& value);

    /// Return the status of the edit.
    editStatus status() const
    {
        return m_status;
    }

private:
    /// Status of the current edit
    editStatus m_status;
};

#endif // EDITOR_H
