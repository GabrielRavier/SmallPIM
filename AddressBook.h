#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <set>
#include <map>

#include "Address.h"

class AddressBook
{
    // Shorthand name for multiset type
    typedef std::multiset<Address>                addrByName_t;
    typedef std::map<int, addrByName_t::iterator> addrByID_t;

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
    // Return number of records found with specified name
    int countName(const std::string& lastname,
                  const std::string& firstname) const;

    // Iterator to traverse address records;
    typedef addrByName_t::const_iterator const_iterator;
    typedef addrByName_t addrlist;

    // Functions to traverse all address records
    const_iterator begin() const
    {
        return m_addresses.begin();
    }

    const_iterator end() const
    {
        return m_addresses.end();
    }

    /* Find first Address with name greater-or-equal to specified name.
       Usually, this will be a name that starts with the specified strings.
    */
    const_iterator findNameStartsWith(const std::string& lastname,
                                      const std::string& firstname="") const;

    /* Find next Address in which any field contains the specified string.
       Indicate starting point for search with start parameter.
    */
    const_iterator findNextContains(const std::string& searchStr,
                                    const_iterator start) const;

    // Return iterator to specified records ID.
    const_iterator findRecordID(int recordID) const
        throw (AddressNotFound);

    // Test routine to print out contents of address book
    void print() const;

private:
    // Disable copying
    AddressBook(const AddressBook&);
    AddressBook& operator=(const AddressBook&);

    static int m_nextID;

    addrByName_t m_addresses;
    addrByID_t   m_addrByID;

    // Get the index of the record with the specified ID.
    // Returns NULL if not found.
    addrByName_t::iterator  getByID(int recordID)
    throw (AddressNotFound);
    addrByName_t::const_iterator  getByID(int recordID) const
    throw (AddressNotFound);
};

#endif // ADDRESSBOOK_H
