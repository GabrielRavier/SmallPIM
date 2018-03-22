#include "AddressBook.h"

#include <algorithm>

using std::count;
using std::string;
using std::binary_function;
using std::count_if;
using std::bind2nd;
using std::lower_bound;
using std::unary_function;
using std::find_if;
using std::make_pair;

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
    else if (m_addrByID.count(recordID))
        // recordID is already in map
        throw DuplicateID();

    // Assign recordID to copy of Address
    Address addrCopy(addr);
    addrCopy.recordID(recordID);

    // Insert record into set
    addrByName_t::iterator i = m_addresses.insert(addrCopy);

    // Insert Address iterator into ID-based map
    // m_addrByID.insert(std::make_pair(recordID, i));
    m_addrByID[recordID] = i;

    return recordID;
}

AddressBook::addrlist::iterator
AddressBook::getByID(int recordID) throw (AddressNotFound)
{
    // Find record by ID
    addrByID_t::iterator IDIter = m_addrByID.find(recordID);
    if (IDIter == m_addrByID.end())
        throw AddressNotFound();

    return IDIter->second;
}

AddressBook::addrlist::const_iterator
AddressBook::getByID(int recordID) const throw (AddressNotFound)
{
    // Find record by ID
    addrByID_t::const_iterator IDIter = m_addrByID.find(recordID);
    if (IDIter == m_addrByID.end())
        throw AddressNotFound();

    return IDIter->second;
}

void AddressBook::eraseAddress(int recordID)
throw (AddressNotFound)
{
    addrlist::iterator i = getByID(recordID);

    // Remove entry from both containers
    m_addresses.erase(i);
    m_addrByID.erase(recordID);
}

void AddressBook::replaceAddress(const Address& addr, int recordID)
throw (AddressNotFound)
{
    if (!recordID)
        recordID = addr.recordID();

    eraseAddress(recordID);
    insertAddress(addr, recordID);
}

const Address& AddressBook::getAddress(int recordID)
throw (AddressNotFound)
{
    return *getByID(recordID);
}

// Function object to compare the name fields of two Address objects
struct AddressNameEqual :
    public binary_function<Address, Address, bool>
{
    bool operator()(const Address& a1, const Address& a2) const
    {
        // Return true if names match
        return (a1.lastname() == a2.lastname() &&
                a1.firstname() == a2.lastname());
    }
};

int AddressBook::countName(const string& lastname,
                           const string& firstname) const
{
    Address searchAddr;
    searchAddr.lastname(lastname);
    searchAddr.firstname(firstname);

    // Return a count of the number of matching records
    return m_addresses.count(searchAddr);
}

/* Find first Address with name greater-or-equal to specified name.
   Usually, this will be a name that starts with the specified strings. */

AddressBook::const_iterator
AddressBook::findNameStartsWith(const string& lastname,
                                const string& firstname) const
{
    Address searchAddr;
    searchAddr.lastname(lastname);
    searchAddr.firstname(firstname);

    return m_addresses.lower_bound(searchAddr);
}

// Function object class to search for a string within an Address.
class AddressContainsStr : public unary_function<Address, bool>
{
public:
    AddressContainsStr(const string& str) : m_str(str) { }

    bool operator()(const Address& a)
    {
        // Return true if any Address field contains m_str
        return (a.lastname().find(m_str) != string::npos ||
                a.firstname().find(m_str) != string::npos ||
                a.phone().find(m_str) != string::npos ||
                a.address().find(m_str) != string::npos);
    }

private:
    string m_str;
};

/* Find next Address in which any field contains the specified string.
   Indicate starting point for search with start parameter. */
AddressBook::const_iterator
AddressBook::findNextContains(const string& searchStr,
                              const_iterator start) const
{
    return find_if(start, m_addresses.end(),
                   AddressContainsStr(searchStr));
}

// Return iterator to specified records ID.
AddressBook::const_iterator
AddressBook::findRecordID(int recordID) const throw (AddressNotFound)
{
    return getByID(recordID);
}
