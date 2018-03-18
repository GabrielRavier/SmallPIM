#include <iostream>

#include "AddressBook.h"

using std::cout;
using std::endl;
using std::list;

int AddressBook::m_nextID = 1;

AddressBook::AddressBook()
{
}

AddressBook::~AddressBook()
{
}

int AddressBook::insertAddress(const Address& addr,
                               int recordID) throw (DuplicateID)
{
    if (!recordID)
        // If recordID is not specified, create a new record ID.
        recordID = m_nextID++;
    else if (recordID >= m_nextID)
        // Make sure nextID is always higher than any known record ID.
        m_nextID = recordID + 1;
    else
    {
        // Make sure we don't have a duplicate ID
        for (list<Address>::iterator i = m_addresses.begin(); i != m_addresses.end(); ++i)
            if (i->recordID() == recordID)
                // Explicitly-specified ID is not unique
                throw DuplicateID();
    }

    // Append record onto vector.
    m_addresses.push_back(addr);

    // Assign an ID to the record
    m_addresses.back().recordID(recordID);

    return recordID;
}

AddressBook::addrlist::iterator
AddressBook::getByID(int recordID) throw (AddressNotFound)
{
    for (addrlist::iterator i = m_addresses.begin();
         i != m_addresses.end(); ++i)
        if (i->recordID() == recordID)
            return i;

    throw AddressNotFound();
}

AddressBook::addrlist::const_iterator
AddressBook::getByID(int recordID) const throw (AddressNotFound)
{
    for (addrlist::const_iterator i = m_addresses.begin();
         i != m_addresses.end(); ++i)
        if (i->recordID() == recordID)
            return i;

    throw AddressNotFound();
}

void AddressBook::eraseAddress(int recordID)
throw (AddressNotFound)
{
    addrlist::iterator i = getByID(recordID);
    m_addresses.erase(i);
}

void AddressBook::replaceAddress(const Address& addr, int recordID)
throw (AddressNotFound)
{
    if (!recordID)
        recordID = addr.recordID();

    addrlist::iterator i = getByID(recordID);

    *i = addr;
    i->recordID(recordID);
}

const Address& AddressBook::getAddress(int recordID)
throw (AddressNotFound)
{
    return *getByID(recordID);
}

void AddressBook::print() const
{
    for (addrlist::const_iterator i = m_addresses.begin();
         i != m_addresses.end(); ++i)
    {
        const Address& a = *i;
        cout << "Record ID: " << a.recordID() << '\n'
             << a.firstname() << ' ' << a.lastname() << '\n'
             << a.address() << '\n' << a.phone() << '\n'
             << endl;
    }
}
