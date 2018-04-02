#include <iostream>
#include <cstdlib>

#include "PIMData.h"
#include "AddressBookMenu.h"
#include "DateBookMenu.h"
#include "MainMenu.h"

// Test function to generate addresses (in TestData.cpp)
extern void generateAddresses(AddressBook& addrbook, int numAddresses);

// Test function to generate appointments (in TestData.cpp)
extern void generateAppointments(DateBook& dateBook, int numDays);

// Global data object
PIMData myPIMData;

using std::auto_ptr;
using std::cout;
using std::endl;
using std::cerr;

int main()
{
    try
    {
        // Create address book and date book
        auto_ptr<AddressBook>   addrBookPtr(new AddressBook);
        auto_ptr<DateBook>      dateBookPtr(new DateBook);

        // Will only get here if no exception was thrown
        myPIMData.addressBook(addrBookPtr);
        myPIMData.dateBook(dateBookPtr);
    }
    catch (...)
    {
        cerr << "Could not create address and date books.\n";
        exit(EXIT_FAILURE);
    }

#ifndef NOGENERATE
    // Generate 50 random address-book entries
    generateAddresses(myPIMData.addressBook(), 50);

    // Generate a year's worth of appointments
    generateAppointments(myPIMData.dateBook(), 366);
#endif // NOGENERATE

    // Create address book menu and date book menus
    AddressBookMenu addrBookMenu(myPIMData.addressBook());
    DateBookMenuCatalog dateBookMenus(myPIMData.dateBook());

    // Create the main menu and push it on the menu stack
    MainMenu mainMenu(&addrBookMenu, dateBookMenus.monthlyMenu());
    Menu::enterMenu(&mainMenu);

    // Process menu choices until menu exits.
    while (Menu::isActive())
        Menu::activeMenu()->mainLoop();

    cout << "\nThank you for using SmallPIM!\n" << endl;

    return 0;
}
