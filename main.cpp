#include <iostream>

#include "Address.h"

using std::cout;
using std::endl;

void dump(const Address& a)
{
    cout << a.firstname() << ' '
         << a.lastname() << '\n'
         << a.address() << '\n'
         << a.phone() << '\n'
         << endl;
}

int main()
{
    Address a;
    a.lastname("Gabriel");
    a.firstname("Ravier");
    a.phone("(617) 555-9876");
    a.address("The GDN Company\nSomewhere, MA 01000");
    dump(a);

    // Add phone extension
    a.phone("(617) 555-7777 ext. 112");
    dump(a);

    return 0;
}
