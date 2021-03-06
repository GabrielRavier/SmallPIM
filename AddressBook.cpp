#include "AddressBook.h"

#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::count;
using std::string;
using std::binary_function;
using std::count_if;
using std::bind2nd;
using std::lower_bound;
using std::unary_function;
using std::find_if;
using std::make_pair;
using std::lexicographical_compare;
using std::tolower;
using std::toupper;

/// Non-case-sensitive character less-than function object
struct ci_less_char : public binary_function<char, char, bool>
{
    bool operator()(char c1, char c2) const
    {
        return toupper(c1) < toupper(c2);
    }
};

/// Algorithm for non-case-sensitive lexicographical comparison of two character sequences
template <class FwdIter1, class FwdIter2>
bool ci_less(FwdIter1 b1, FwdIter1 e1, FwdIter2 b2, FwdIter2 e2)
{
    return lexicographical_compare(b1, e1, b2, e2, ci_less_char());
}

/// Non-case-sensitive string comparison function
bool ciStringLess(const string& s1, const string& s2)
{
    return ci_less(s1.begin(), s1.end(), s2.begin(), s2.end());
}

bool AddressLess::operator ()(const Address& a1, const Address& a2) const
{
    if (ciStringLess(a1.lastname(), a2.lastname()))
        return true;
    else if (ciStringLess(a2.lastname(), a1.lastname()))
        return false;
    else
        return ciStringLess(a1.firstname(), a2.firstname());
}

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

AddressBook::addrByName_t::iterator AddressBook::getByID(int recordID) throw (AddressNotFound)
{
    // Find record by ID
    addrByID_t::iterator IDIter = m_addrByID.find(recordID);
    if (IDIter == m_addrByID.end())
        throw AddressNotFound();

    return IDIter->second;
}

AddressBook::addrByName_t::const_iterator AddressBook::getByID(int recordID) const throw (AddressNotFound)
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
    addrByName_t::iterator i = getByID(recordID);

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

/// Function object to compare the name fields of two Address objects
struct AddressNameEqual :
    public binary_function<Address, Address, bool>
{
    /// Returns true if names match
    bool operator()(const Address& a1, const Address& a2) const
    {
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
AddressBook::const_iterator AddressBook::findNameStartsWith(const string& lastname,
                                const string& firstname) const
{
    Address searchAddr;
    searchAddr.lastname(lastname);
    searchAddr.firstname(firstname);

    return m_addresses.lower_bound(searchAddr);
}

/// Function object class to search for a string within an Address.
class AddressContainsStr : public unary_function<Address, bool>
{
public:
    AddressContainsStr(const string& str) : m_str(str) { }


    /// Returns true if any Address field contains m_str
    bool operator()(const Address& a)
    {
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
AddressBook::const_iterator AddressBook::findNextContains(const string& searchStr,
                              const_iterator start) const
{
    return find_if(start, m_addresses.end(),
                   AddressContainsStr(searchStr));
}

// Return iterator to specified records ID.
AddressBook::const_iterator AddressBook::findRecordID(int recordID) const throw (AddressNotFound)
{
    return getByID(recordID);
}

void AddressBook::print() const
{
    cout << "******************************************\n";
    for (addrByName_t::const_iterator i = m_addresses.begin(); i != m_addresses.end(); ++i)
    {
        const Address& a = *i;
        cout << "Record ID : " << a.recordID() << '\n' << a.firstname() << ' ' << a.lastname() << '\n' << a.address() << '\n'
             << a.phone() << '\n' << endl;
    }
}
