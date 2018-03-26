#include <iostream>

#include "AddressBook.h"
#include "AddressBookMenu.h"

extern void generateAddresses(AddressBook& addrbook, int numAddresses);

using std::cout;
using std::endl;

// Main program just calls main menu, for now.
int main()
{
    AddressBook addrBook;

    // Generate 50 random address-book entries
    generateAddresses(addrBook, 50);

    // Create address book menu and push on menu stack.
    AddressBookMenu addrBookMenu(addrBook);
    Menu::enterMenu(&addrBookMenu);

    // Process menu choices until menu exits.
    while (Menu::isActive())
        Menu::activeMenu()->mainLoop();

    cout << "\nThank you for using SmallPIM !\n" << endl;

    return 0;
}
