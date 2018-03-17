#include <iostream>

#include "AddressBook.h"

using std::cout;
using std::endl;

int main()
{
    AddressBook book;

    Address a;
    a.firstname("Gabriel");
    a.lastname("Ravier");
    a.phone("(617) 555-9876");
    a.address("The GDN Company\nSomewhere, MA 01000");

    Address b;
    b.firstname("Blargo");
    b.lastname("Balrog");
    b.phone("(CS) 555-CAVE");
    b.address("Nicalis\nSomewhere idk lol");

    Address c;
    c.lastname("Neighborhood Video");
    c.phone("555-FILM");

    int a_ID = book.insertAddress(a);
    int b_ID = book.insertAddress(b);
    int c_ID = book.insertAddress(c);

    cout << "*** Three Address Entries ***\n";
    book.print();

    // Address d has same name as b
    Address d;
    d.firstname("Blargo");
    d.lastname("Balrog");
    d.phone("555-STORY");
    d.address("1 Small St.\nMarlboro, MA 02100");

    // Insert address with duplicate name
    int d_ID = book.insertAddress(d);
    cout << "*** After inserting a duplicate Blargo Balrog ***\n";
    book.print();

    // Erase an address
    book.eraseAddress(a_ID);
    cout << "*** After erasing Gabriel Ravier ***\n";
    book.print();

    // Replace an address
    c.address("22 Main St.\n");
    book.replaceAddress(c, c_ID);
    cout << "*** After replacing Neighborhood Video ***\n";
    book.print();

    // Get and print address
    const Address& d2 = book.getAddress(d_ID);
    cout << "*** Copy of d: ***\n"
         << d2.firstname() << ' ' << d2.lastname() << '\n'
         << d2.address() << '\n' << d2.phone() << '\n'
         << endl;

    return 0;
}
