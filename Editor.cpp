#include "Editor.h"

#include <iostream>

using std::string;
using std::cout;
using std::getline;
using std::cin;

bool Editor::editSingleLine(const string& prompt, string& value)
{
    m_status = normal;
    cout << prompt;
    if (! value.empty())
        cout << " [" << value << ']';
    cout << ": ";

    string result;
    getline(cin, result);

    // Clear input state and abort edit if an input error occurred
    if (cin.fail())
    {
        m_status = canceled;
        return false;
    }

    // If user pressed return key without entering anything, leave value unchanged.
    if (result.empty())
        return true;

    // If user entered a period by itself, then end edit.
    if (result == ".")
    {
        m_status = finished;
        return false;
    }

    // If user entered the abort code, then abort.
    if (result == "!x")
    {
        m_status = canceled;
        return false;
    }

    // Return new string
    value = result;
    return true;
}

bool Editor::editMultiLine(const string& prompt, string& value)
{
    m_status = normal;

    /*  Append a newline to value. This makes the rest of the code simpler by making every line end with newline.
        The extra newline is stripped off at the end.
    */
    value += '\n';

    string::size_type lineBegin = 0, lineEnd = 0, lineLen = 0;
    for (;;)
    {
        // If at end of string, append a new line
        if (lineBegin >= value.length())
            value += '\n';

        // Extract the next line from value
        string line;
        lineEnd = value.find('\n', lineBegin);  // Should always succeed
        lineLen = lineEnd - lineBegin;
        line = value.substr(lineBegin, lineLen);

        // Edit line
        if (!editSingleLine(prompt, line))
            break;

        // Check for special sequences
        if (line == "!n")
            break; // User has signaled end of multi-line input

        if (line == "!i")
        {
            // User wants to insert a line.
            value.insert(lineBegin, "\n");  // Insert an empty line
            continue;   // Loop around and edit the empty line
        }

        if (line == "!d")
        {
            // User wants to delete a line. Erase line and the newline after it.
            value.erase(lineBegin, lineLen + 1);
            continue;   // Loop around and edit the next line
        }

        // Replace line with new value
        value.replace(lineBegin, lineLen, line);

        // Advance position past inserted line and past newline character.
        lineBegin += line.length() + 1;
    }

    // Editing is done. Strip extra newlines off end of string
    string::size_type strip = value.find_last_not_of('\n') + 1;
    value.erase(strip, string::npos);

    return (status() == normal);
}
