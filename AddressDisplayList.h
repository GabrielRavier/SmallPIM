#ifndef ADDRESSDISPLAYLIST_H
#define ADDRESSDISPLAYLIST_H

#include "DisplayList.h"
#include <string>
#include <vector>

class AddressBook;

/// Specialized DisplayList for Address records.
class AddressDisplayList : public DisplayList
{
public:
    /// Construct with a reference to the address book
    AddressDisplayList(AddressBook& addrBook);

    /** Scroll to first Address with name greater-than-or-equal to specified name. Usually, this will be a name that starts
       with the specified strings. Returns false if no match found. */
    bool findNameStartsWith(const std::string& lastname, const std::string& firstname = "");

    /// List only those records that contain the specified string
    void listContainsString(const std::string&);

    /// List all records (use after a listContainsString)
    void listAll();

protected:
    /// Display the specified address record in one-line format. Implements pure virtual base-class function.
    virtual void displayRecord(int recordID);

    /// Override base-class function to retrieve more records.
    virtual bool fetchMore(int startID, int numRecords, std::vector<int>& result);

private:
    /// Address book to be displayed
    AddressBook&    m_addressBook;

    /// String to use for listContainsString mode.
    std::string     m_containsString;
};

#endif // ADDRESSDISPLAYLIST_H
