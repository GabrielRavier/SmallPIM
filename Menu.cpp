#include "Menu.h"

#include <cctype>
#include <iostream>

using std::tolower;
using std::toupper;

using std::string;
using std::cin;
using std::cout;
using std::flush;

/* Display a menu string and then allow user to enter a character from withing a string of choices.
*  If user enters a character not in the choices string, an error is printed and the user is prompted to try again.
*
* \param menu : String to be displayed
* \param choices : Choices available to the user
* \return After a valid character is entered, the selected character is returned ('\0' is returned in the case of an I/O error)
*
*/
char Menu::getMenuSelection(const std::string& menu, const std::string& choices)
{
    while (cin.good())
    {
        cout << menu;

        char selection = '\0';
        cin >> selection;
        if (cin.fail())
            break;

        // Throw away rest of input line
        cin.ignore(10000, '\n');

        // Search for selection in either uppercase and lowercase
        if (choices.find(toupper(selection)) != string::npos || choices.find(tolower(selection)) != string::npos)
            return toupper(selection);  // Valid entry
        else
            cout << "Invalid selection, please try again.\n\n";
    }

    return '\0';
}

// Clear the screen
void Menu::clearScreen()
{
    // Because not all terminals respond to the formfeed character to clear the screen, we also output 25 newlines
    cout << "\f\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << flush;
}

// Define m_menuStack member variable
std::stack<Menu*> Menu::m_menuStack;
