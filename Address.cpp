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
    strncpy(m_lastname, source, namelen - 1);
    m_lastname[namelen - 1] = '\0';
}

void Address::firstname(const char *source)
{
    strncpy(m_firstname, source, namelen - 1);
    m_firstname[namelen - 1] = '\0';
}

void Address::phone(const char *source)
{
    strncpy(m_phone, source, phonelen - 1);
    m_phone[phonelen - 1] = '\0';
}

void Address::address(const char *source)
{
    strncpy(m_address, source, addrlen);
    m_address[addrlen - 1] = '\0';
}
