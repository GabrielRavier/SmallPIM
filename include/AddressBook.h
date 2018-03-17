#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <list>

#include "Address.h"

class AddressBook
{
public:
    AddressBook();
    ~AddressBook();

    // Exception classes
    class AddressNotFound {};
    class DuplicateID {};

    int insertAddress(const Address& addr, int recordID = 0) // recordID optional
    throw (DuplicateID);
    void eraseAddress(int recordID) throw (AddressNotFound);
    void replaceAddress(const Address& addr, int recordID = 0)
    throw (AddressNotFound);
    const Address& getAddress(int recordID) const
    throw (AddressNotFound);

    // Test routine to print out contents of address book
    void print() const;

private:
    // Disable copying
    AddressBook(const AddressBook&);
    AddressBook& operator=(const AddressBook&);

    static int m_nextID;
    std::list<Address> m_addresses;
};

#endif // ADDRESSBOOK_H
