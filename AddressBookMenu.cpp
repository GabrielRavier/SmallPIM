#include "AddressBookMenu.h"

#include <iostream>
#include <iomanip>
#include <climits>

#include "Address.h"
#include "AddressBook.h"
#include "AddressEditor.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;

void AddressBookMenu::mainLoop()
{
    clearScreen();
    cout << "*** Address Book ***\n\n";

    m_displayList.display();
    cout << '\n';

    static const char menu[] =
        "(P)revious, (N)ext, (V)iew, (C)reate, (D)elete, (E)dit, \n"
        "list (A)ll, (L)ookup, (S)earch, (R)edisplay, (Q)uit ? ";

    static const char choices[] = "PNVCDEALSRQ";

    switch (getMenuSelection(menu, choices))
    {
    case 'P':
        m_displayList.pageUp();
        break;
    case 'N':
        m_displayList.pageDown();
        break;
    case 'V':
        viewEntry();
        break;
    case 'C':
        createEntry();
        break;
    case 'D':
        deleteEntry();
        break;
    case 'E':
        editEntry();
        break;
    case 'A':
        listAll();
    case 'L':
        lookup();
    case 'S':
        search();
    case 'R':
        // Do nothing, just loop
        break;
    case 'Q':
        exitMenu();
        break;
    default:
        exitMenu();
        break;
    }
}

void AddressBookMenu::viewEntry()
{
    // Ask the display list for a record selection
    int recordID = m_displayList.selectRecord();
    if (!recordID)
        return;

    Address addr = m_addressBook.getAddress(recordID);
    cout << "\nName : " << addr.lastname();
    if (!addr.firstname().empty())
        cout << ", " << addr.firstname();
    cout << "\nPhone : " << addr.firstname();
    cout << "\nAddress : " << addr.address();

    cout << "\n\nPress [RETURN] when ready.";
    cin.ignore(INT_MAX, '\n');
}

void AddressBookMenu::createEntry()
{
    // Edit an empty address
    AddressEditor editor;
    Address addr;

    // Continue editing until a record is saved or canceled
    while(editor.edit())
    {
        addr = editor.addr();
        if (addr.lastname().empty())
        {
            cout << "Last name must not be empty." << endl;
            continue;   // Loop and re-edit
        }

        // Search for existing entries with the same name
        int duplicates = m_addressBook.countName(addr.lastname(), addr.firstname());


        int recordID = 0;
        if (!duplicates)
        {
            // No duplicates
            recordID = m_addressBook.insertAddress(addr);

            // Scroll to display new record
            m_displayList.scrollToTop(recordID);
            return;
        }
        else
        {
            // Duplicate name. Check to see what user wants to do
            cout << "There are already " << duplicates << " records with that name.\n";
            switch (getMenuSelection("(S)ave as new record, (E)dit record or (C)ancel ?", "SEC"))
            {
            case 'S':   // Save record (create a duplicate)
                recordID = m_addressBook.insertAddress(addr);
                m_displayList.scrollToTop(recordID);
                break;

            case 'E':   // Edit record again
                continue;   // Loop back and re-edit

            case 'C':   // Cancel or
            default:    // I/0 error
                return;

            }
        }
    }
}

void AddressBookMenu::deleteEntry()
{
    int recordID = m_displayList.selectRecord();
    if (!recordID)
        return;

    // Find first visible record on screen. If it is the record we are erasing, then find second visible record on screen
    int firstVisible = m_displayList.screenRecord(0);
    if (firstVisible == recordID)
        firstVisible = m_displayList.screenRecord(1);

    // Erase the address
    m_addressBook.eraseAddress(recordID);

    // Deleting the entry invalidates the display list cache. Reset it, then scroll back to the previous position
    m_displayList.reset();
    if (firstVisible != 0)
        m_displayList.scrollToTop(firstVisible);
}

void AddressBookMenu::editEntry()
{
    int recordID = m_displayList.selectRecord();
    if (!recordID)
        return;

    // Create an editor for the selected address
    Address addr = m_addressBook.getAddress(recordID);
    AddressEditor editor(addr);

    // Edit the address
    if (editor.edit())
    {
        // Replace address with modified version
        m_addressBook.replaceAddress(editor.addr());

        // Address's sort order might have changed. We need to reset the display list.
        m_displayList.reset();

        // Scroll modified item to top of screen
        m_displayList.scrollToTop(recordID);
    }
}

void AddressBookMenu::lookup()
{
    // Prompt for lastname and (optional) firstname
    string lookup_name;
    cout << "Lookup name (lastname [, firstname]): ";
    getline(cin, lookup_name);
    if (lookup_name.empty())
        return;

    // Find end of last name and start of first name
    string::size_type lastNameEnd = lookup_name.find(',');
    string::size_type firstNameStart = string::npos;
    if (lastNameEnd != string::npos)
        firstNameStart = lookup_name.find_first_not_of(", \t\f\n\v", lastNameEnd);

    if (firstNameStart == string::npos)
        // Lookup using last name only
        m_displayList.findNameStartsWith(lookup_name.substr(0, lastNameEnd));
    else
        m_displayList.findNameStartsWith(lookup_name.substr(0, lastNameEnd), lookup_name.substr(firstNameStart));
}

void AddressBookMenu::search()
{
    string searchString;
    cout << "Search for string : ";
    getline(cin, searchString);
    if (searchString.empty())
        return;

    m_displayList.listContainsString(searchString);
}

void AddressBookMenu::listAll()
{
    m_displayList.listAll();
}
