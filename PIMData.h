#ifndef PIMDATA_H
#define PIMDATA_H

#include <memory>
#include "AddressBook.h"
#include "DateBook.h"

/// Class to encapsulate all of the data for a given PIM file
class PIMData
{
public:
    PIMData()
    {

    }

    /// \return Address book contained in PIMData
    AddressBook& addressBook()
    {
        return *m_addressBook;
    }

    /// \return Date book contained in PIMData
    DateBook& dateBook()
    {
        return *m_dateBook;
    }

    /** Change address book contained in PIMData
        \param ab : New address book */
    void addressBook(std::auto_ptr<AddressBook> ab)
    {
        m_addressBook = ab;
    }

    /** Change date book contained in PIMData
        \param db : New date book */
    void dateBook(std::auto_ptr<DateBook> db)
    {
        m_dateBook = db;
    }

private:
    std::auto_ptr<AddressBook>  m_addressBook;
    std::auto_ptr<DateBook>     m_dateBook;

    /// Because this class contains auto_ptrs, it does not have proper copy semantics. We disable copying to avoid problems.
    PIMData(const PIMData&);
    PIMData& operator=(const PIMData&);
};

#endif // PIMDATA_H
