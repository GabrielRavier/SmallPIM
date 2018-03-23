#include "AddressDisplayList.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "Address.h"
#include "AddressBook.h"

using std::string;
using std::cout;
using std::setfill;
using std::left;
using std::setw;
using std::vector;
using std::reverse;

// Construct with a reference to the address book
AddressDisplayList::AddressDisplayList(AddressBook& addrBook)
    : m_addressBook(addrBook)
{

}

// Display the specified address record in one-line format
void AddressDisplayList::displayRecord(int recordID)
{
    Address record = m_addressBook.getAddress(recordID);

    // Create a name string in "lastname, firstname" format.
    string name = record.lastname();
    if (! record.firstname().empty())
        name.append(", ").append(record.firstname());

    // Output name and phone number on one line (two columns)
    cout << setfill('.') << setw(40) << left << name << record.phone();
}

bool AddressDisplayList::fetchMore(int startID, int numRecords, vector<int>& result)
{
    // Remove old contents of result
    result.clear();

    if (numRecords == 0)
        return false;

    bool forwards = true;
    if (numRecords < 0)
    {
        forwards = false;
        numRecords = -numRecords;
    }

    // Check for empty list
    if (m_addressBook.begin() == m_addressBook.end())
        return true;

    // Declare an iterator
    AddressBook::const_iterator iter;

    /* Get iterator to record specified by startID. When fetching forward, increment iterator past matching record to
       avoid a duplicate insertion into the display list. */
    if (!startID)
        iter = (forwards ? m_addressBook.begin() : m_addressBook.end());
    else
    {
        iter = m_addressBook.findRecordID(startID);
        if (forwards)
            ++iter;
    }

    if (m_containsString.empty())
    {
        // "List all" mode

        if (forwards)
        {
            // Retrieve records starting at iter
            while (iter != m_addressBook.end() && numRecords-- > 0)
                result.push_back((iter++)->recordID());

            // Return true if reached end of the list
            return iter == m_addressBook.end();
        }
        else
        {
            // Retrive records starting at one before iter
            while (iter != m_addressBook.begin() && numRecords-- > 0)
                result.push_back((--iter)->recordID());

            // Records were pushed backwards, reverse them here:
            reverse(result.begin(), result.end());

            // Return true if reached front of the list
            return iter == m_addressBook.begin();
        }
    }
    else
    {
        // "Contains string" mode

        if (forwards)
        {
            // Retrieve records AFTER startID

            // Find matching record starting at iter
            iter = m_addressBook.findNextContains(m_containsString, iter);
            while (iter != m_addressBook.end() && numRecords-- > 0)
            {
                result.push_back(iter->recordID());

                // Find next matching record
                iter = m_addressBook.findNextContains(m_containsString, ++iter);
            }

            // Return true if we reached the end
            return iter == m_addressBook.end();
        }
        else
        {
            // Retrive records BEFORE startID

            // AddressBook does not have a function to search backwards. Instead, we retrieve ALL records before iter
            AddressBook::const_iterator endIter = iter;
            iter = m_addressBook.findNextContains(m_containsString, m_addressBook.begin());

            while (*iter < *endIter)
            {
                result.push_back(iter->recordID());
                iter = m_addressBook.findNextContains(m_containsString, ++iter);
            }

            return true;    // Yes, we reached the start of the list
        }
    }
}

/* Scroll to first Address with name greater-than-or-equal to specified name. Usually, this will be a name that starts with
   the specified strings. Returns false if no match found. */
bool AddressDisplayList::findNameStartsWith(const string& lastname, const string& firstname)
{
    m_containsString = ""; // Turn off "contains string" mode

    reset();
    AddressBook::const_iterator iter = m_addressBook.findNameStartsWith(lastname, firstname);

    if (iter == m_addressBook.end())
        return false;

    // Scroll found record to top
    scrollToTop(iter->recordID());
    return true;
}

// List only those records that contain the specified string
void AddressDisplayList::listContainsString(const string& s)
{

}
