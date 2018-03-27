#include <iostream>
#include <cstdlib>
#include <iomanip>

#include "AddressBook.h"
#include "AddressBookMenu.h"
#include "AddressEditor.h"
#include "DateTime.h"

extern void generateAddresses(AddressBook& addrbook, int numAddresses);

using std::cout;
using std::cin;
using std::endl;
using std::setfill;
using std::left;
using std::setw;
using std::hex;

void dump_address(const Address& a)
{
    cout << "Record " << a.recordID() << '\n' << a.firstname() << ' ' << a.lastname() << '\n' << a.address() << '\n'
         << a.phone() << '\n' << endl;
}

void address_test()
{
    Address a;
    a.lastname("Ravier");
    a.firstname("Gabriel");
    a.phone("01 626472213");
    a.address("5 Niederhochstadter Strasse\nSteinbach, Germany");
    dump_address(a);

    // Add country extension
    a.phone("+49 1626472213");
    cout << "After adding country extension : \n\n";
    dump_address(a);
}

void addressbook_test()
{
    AddressBook book;

    Address a;
    a.lastname("Ravier");
    a.firstname("Gabriel");
    a.phone("01 626472213");
    a.address("5 Niederhochstadter Strasse\nSteinbach, Germany");

    Address b;
    b.lastname("Sarnikowski");
    b.firstname("Gilles");
    b.phone("Idk lol");
    b.address("Acheres");

    Address c;
    c.lastname("Neighborhood Video");
    c.phone("01 324353234");

    int a_ID = book.insertAddress(a);
    // int b_ID = book.insertAddress(b);
    int c_ID = book.insertAddress(c);

    cout << "*** Three Address Entries ***\n";
    book.print();

    // Address d has same name as b
    Address d;
    d.lastname("Sarnikowski");
    d.firstname("Gilles");
    d.phone("04 BLARGO");
    d.address("Blourga, France");

    // Insert address with duplicate name
    int d_ID = book.insertAddress(d);
    cout << "*** After adding a duplicate Gilles Sarnikowski ***\n";
    book.print();

    // Erase an address
    book.eraseAddress(a_ID);
    cout << "*** After erasing Gabriel Ravier ***\n";
    book.print();

    // Replace an address
    c.address("666 Main Street.\nLol Town, USA");
    book.replaceAddress(c, c_ID);
    cout << "*** After replacing Neighborhood Video ***\n";
    book.print();

    // Get and print address
    const Address& d2 = book.getAddress(d_ID);
    cout << "*** Copy of d : ***\n" << d2.firstname() << ' ' << d2.lastname() << '\n' << d2.address() << '\n' << d2.phone()
         << '\n' << endl;
}

void addresseditor_test()
{
    Address a;

    AddressEditor editor(a);
    cout << "Make last name equal to \"done\" when finished\n";
    while (a.lastname() != "done")
    {
        editor.edit();
        a = editor.addr();
        cout << endl;
        dump_address(a);
    }
}

void datetime_test()
{
    // Test now() function
    cout << "Now = " << DateTime::now() << endl;

    // Test constructor
    cout << "Something = " << DateTime(0, 1, 1, 0, 0) << endl;

    DateTime dt;

    while (cin)
    {
        cout << "\nEnter a date and time (Q to quit) : ";
        if (cin.peek() == 'q' || cin.peek() == 'Q')
            break;

        cin >> dt;
        if (cin.fail())
        {
            cout << "Bad input" << endl;
            cin.clear();
        }
        else
        {
            cout << "DateTime = " << dt << endl;
            cout << setfill('*') << left << setw(30) << hex << dt << ' ' << setw(10) << 0xff << endl;
        }

        cin.ignore(INT_MAX, '\n');  // If fails, loop will exit
    }
}

// Main program just calls main menu, for now.
int main()
{
    cout << "Doing address test...\n" << endl;
    address_test();
    system("pause");

    cout << "Doing address book test...\n" << endl;
    system("pause");
    addressbook_test();

    cout << "Doing address editor test...\n" << endl;
    system("pause");
    addresseditor_test();

    cout << "Doing date time test ...\n" << endl;
    system("pause");
    datetime_test();
    cin.ignore(INT_MAX, '\n');  // Avoid error in cin for rand addresses

    AddressBook addrBook;

    bool do_addresses;
    cout << "Want to generate random addresses ? (1 for yes, 0 for no)" << endl;
    cin >> do_addresses;
    cin.ignore(INT_MAX, '\n');  // Avoid error in cin
    if (do_addresses)
        // Generate 50 random address-book entries
        generateAddresses(addrBook, 50);

    cout << "Starting up address book menu..." << endl;
    system("pause");
    // Create address book menu and push on menu stack.
    AddressBookMenu addrBookMenu(addrBook);
    Menu::enterMenu(&addrBookMenu);

    // Process menu choices until menu exits.
    while (Menu::isActive())
        Menu::activeMenu()->mainLoop();

    cout << "\nThank you for using SmallPIM !\n" << endl;

    return 0;
}
