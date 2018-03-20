#include <iostream>

#include "Address.h"
#include "AddressEditor.h"

using std::cout;
using std::endl;

void dump (const Address& a)
{
    cout << "Record :" << a.recordID() << '\n'
         << a.firstname() << ' ' << a.lastname() << '\n'
         << a.address() << '\n' < a.phone() << '\n' << endl;
}

int main()
{
    Address a;

    AddressEditor editor(a);
    while (a.lastname() != "done")
    {
        editor.edit();
        a = editor.addr();
        cout << endl;
        dump(a);
    }

    return 0;
}
