#include "MainMenu.h"

#include <iostream>
#include "PIMData.h"
#include "MainMenu.h"
#include "AddressBookMenu.h"
#include "DateBookMenu.h"

using std::cout;

void MainMenu::mainLoop()
{
    clearScreen();
    cout << "Welcome to SmallPIM!\n\n" << "*** Main Menu ***\n\n";

    static const char menu[] =
    "Please select from the following : \n\n"
    "  (A)ddress Book\n  (D)ate Book\n  (Q)uit\n\n"
    "Enter selection> ";

    switch (getMenuSelection(menu, "ADQ"))
    {
    case 'A':
        addressBook();
        break;
    case 'D':
        dateBook();
        break;
    case 'Q':
    default:
        quit();
        break;
    }
}

void MainMenu::addressBook()
{
    enterMenu(m_addrBookMenu);
}

void MainMenu::dateBook()
{
    enterMenu(m_dateBookMenu);
}

void MainMenu::quit()
{
    exitMenu();
}
