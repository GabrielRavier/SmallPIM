#include <iostream>

#include "AddressBook.h"

using std::cout;
using std::endl;

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
    else if (getByID(recordID) != notFound)
        // Explicitely-specified ID is not unique
        throw DuplicateID();

    // Append record onto vector.
    m_addresses.push_back(addr);

    // Assign an ID to the record
    m_addresses.back().recordID(recordID);

    return recordID;
}

int AddressBook::getByID(int recordID) const
{
    // Loop through all the elements and compare each element's record ID with the one we're searching for.
    for (unsigned int i = 0; i < m_addresses.size(); ++i)
        if (m_addresses[i].recordID() == recordID)
            return i;

    return notFound;
}

void AddressBook::eraseAddress(int recordID)
throw (AddressNotFound)
{
    // Find address
    int index = getByID(recordID);
    if (index == notFound)
        throw AddressNotFound();

    // Move element from end of vector to location being erased.
    m_addresses[index] = m_addresses.back();

    // Remove the now unused last element of the vector.
    m_addresses.pop_back();
}

void AddressBook::replaceAddress(const Address& addr, int recordID)
throw (AddressNotFound)
{
    if (!recordID)
        recordID = addr.recordID();

    int index = getByID(recordID);
    if (index == notFound)
        throw AddressNotFound();

    m_addresses[index] = addr;
    m_addresses[index].recordID(recordID);
}

const Address& AddressBook::getAddress(int recordID) const
throw (AddressNotFound)
{
    int index = getByID(recordID);
    if (index == notFound)
        throw AddressNotFound();

    return m_addresses[index];
}

void AddressBook::print() const
{
    cout << "******************************************\n";
    for (unsigned int i = 0; i < m_addresses.size(); ++i)
    {
        const Address& a = m_addresses[i];
        cout << "Record ID: " << a.recordID() << '\n'
             << a.firstname() << ' ' << a.lastname() << '\n'
             << a.address() << '\n' << a.phone() << '\n'
             << endl;
    }
}
