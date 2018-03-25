#ifndef MENU_H
#define MENU_H

#include <string>
#include <stack>

/// Base class for menus
class Menu
{
public:
    Menu()
    {

    }
    virtual ~Menu()
    {

    }

    /// Execute the menu's main loop
    virtual void mainLoop() = 0;

    /// Return the (global) currently active menu.
    static Menu* activeMenu()
    {
        return m_menuStack.top();
    }

    /** \brief Enter a menu or submenu
     *
     * \param m : Menu to be entered
     *
     */
    static void enterMenu(Menu* m)
    {
        m_menuStack.push(m);
    }

    /// Exit submenu back to previous-level menu.
    static void exitMenu()
    {
        m_menuStack.pop();
    }

    /// Returns true if there is an active menu
    static bool isActive()
    {
        return !m_menuStack.empty();
    }

protected:
    // Utilities for use by derived classes:

    /** Display a menu string and then allow user to enter a character from withing a string of choices.
     *  If user enters a character not in the choices string, an error is printed and the user is prompted to try again.
     *
     * \param menu : String to be displayed
     * \param choices : Choices available to the user
     * \return After a valid character is entered, the selected character is returned ('\0' is returned in the case of an I/O error)
     *
     */
     static char getMenuSelection(const std::string& menu, const std::string& choices);

     /// Clears the screen
     static void clearScreen();

private:
    /// Stack of menus and sub-menus
    static std::stack<Menu*> m_menuStack;
};

#endif // MENU_H
