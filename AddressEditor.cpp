#include "AddressEditor.h"

#include <iostream>

using std::string;

// Start with an empty Address object
AddressEditor::AddressEditor()
{
}

// Edit an existing Address object
AddressEditor::AddressEditor(const Address& a)
    : m_addr(a)
{
}

// Main loop returns true if address was successfully edited, false if edit was aborted.
bool AddressEditor::edit()
{
    // Unpack the address
    string lastname(m_addr.lastname());
    string firstname(m_addr.firstname());
    string phone(m_addr.phone());
    string address(m_addr.address());

    // Edit each field
    editSingleLine("Last name", lastname) &&
    editSingleLine("First name", firstname) &&
    editSingleLine("Phone Number", phone) &&
    editMultiLine("Address", address);

    if (status() == canceled)
        return false;

    // Commit changes
    m_addr.lastname(lastname);
    m_addr.firstname(firstname);
    m_addr.phone(phone);
    m_addr.address(address);

    return true;
}

