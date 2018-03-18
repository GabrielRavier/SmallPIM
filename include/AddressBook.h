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
    const Address& getAddress(int recordID)
    throw (AddressNotFound);

    // Test routine to print out contents of address book
    void print() const;

private:
    // Disable copying
    AddressBook(const AddressBook&);
    AddressBook& operator=(const AddressBook&);

    static int m_nextID;

    // Define addrlist as a shorthand for std::list<Address>
    typedef std::list<Address> addrlist;
    addrlist m_addresses;

    // Get the index of the record with the specified ID.
    // Returns end() if not found.
    addrlist::iterator  getByID(int recordID)
        throw (AddressNotFound);
    addrlist::const_iterator  getByID(int recordID) const
        throw (AddressNotFound);
};

#endif // ADDRESSBOOK_H
