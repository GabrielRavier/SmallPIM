#include "Address.h"
#include <cstring>

using std::strcpy;

Address::Address()
{
    // Initialise all strings to empty
    m_lastname[0] = m_firstname[0] = m_phone[0] = m_address[0] = '\0';
}

void Address::lastname(const char *source)
{
    strcpy(m_lastname, source);
}

void Address::firstname(const char *source)
{
    strcpy(m_firstname, source);
}

void Address::phone(const char *source)
{
    strcpy(m_lastname, source);
}

void Address::address(const char *source)
{
    strcpy(m_address, source);
}
