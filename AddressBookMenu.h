#ifndef ADDRESSBOOKMENU_H
#define ADDRESSBOOKMENU_H

#include "Menu.h"
#include "AddressDisplayList.h"

/// Menu for an address book
class AddressBookMenu : public Menu
{
public:
    /** Menu for an address book
        \param addrBook : Address book to be used by the menu
    */
    AddressBookMenu(AddressBook& addrBook) : m_addressBook(addrBook), m_displayList(addrBook)
    {

    }

    /// Main loop for the address book menu
    void mainLoop();

private:
    /// View an entry
    void viewEntry();
    /// Create a new entry
    void createEntry();
    /// Edit an entry
    void editEntry();
    /// Delete an entry
    void deleteEntry();
    /// List all address
    void listAll();
    /// Lookup an address
    void lookup();
    /// Search for an address
    void search();

    /// Menu's address book
    AddressBook&        m_addressBook;
    /// Menu's display list
    AddressDisplayList  m_displayList;
};

#endif // ADDRESSBOOKMENU_H
