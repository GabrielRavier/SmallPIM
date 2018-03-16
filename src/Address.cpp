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
