#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <vector>

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
    std::vector<Address> m_addresses;

    // Get the index of the record with the specified Id.
    // Returns notFound if not found.
    int getByID(int recordID) const;

    static const int notFound = -1;
};

#endif // ADDRESSBOOK_H
