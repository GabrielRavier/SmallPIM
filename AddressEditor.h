#ifndef ADDRESSEDITOR_H
#define ADDRESSEDITOR_H

#include "Editor.h"
#include "Address.h"


class AddressEditor : public Editor
{
public:
    /// Start with an empty Address object
    AddressEditor();

    /// Edit an existing Address object
    AddressEditor(const Address& a);

    // Use compiler-generated destructor
    // ~AddressEditor();

    /// Main loop returns true if address was successfully edited, false if edit was aborted
    bool edit();

    /// return m_addr;
    Address addr()
    {
        return m_addr;
    }

    /// m_addr = a;
    void addr(const Address& a)
    {
        m_addr = a;
    }

private:
    /// Disable copying
    AddressEditor(const AddressEditor&);
    const AddressEditor& operator=(const AddressEditor&);

    /// Address to be edited
    Address m_addr;
};

#endif // ADDRESSEDITOR_H
