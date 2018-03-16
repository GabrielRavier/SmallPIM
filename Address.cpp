#include <cstring>

#include "Address.h"

using std::strcpy;
using std::strncpy;

Address::Address()
{
    // Initialise all strings to empty
    m_lastname[0] = m_firstname[0] = m_phone[0] = m_address[0] = '\0';
}

void Address::lastname(const char *source)
{
    strncpy(m_lastname, source, namelen);
}

void Address::firstname(const char *source)
{
    strncpy(m_firstname, source, namelen);
}

void Address::phone(const char *source)
{
    strncpy(m_lastname, source, phonelen);
}

void Address::address(const char *source)
{
    strncpy(m_address, source, addrlen);
}
