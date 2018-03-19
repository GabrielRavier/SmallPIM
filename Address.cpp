#include "Address.h"

using std::string;

void Address::lastname(const string& source)
{
    m_lastname = source;
}

void Address::firstname(const string& source)
{
    m_firstname = source;
}

void Address::phone(const string& source)
{
    m_phone = source;
}

void Address::address(const string& source)
{
    m_address = source;
}

bool operator==(const Address& a1, const Address& a2)
{
    return (a1.lastname() == a2.lastname() &&
            a1.firstname() == a2.firstname() &&
            a1.phone() == a2.phone() &&
            a1.address() == a2.address());
}

bool operator< (const Address& a1, const Address& a2)
{
    if (a1.lastname() < a2.lastname())
        return true;
    else if (a2.lastname() < a1.lastname())
        return false;
    else
        return (a1.firstname() < a2.firstname());
}
