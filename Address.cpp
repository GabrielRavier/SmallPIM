#include <cstring>

#include "Address.h"

using std::strcpy;
using std::strncpy;
using std::strlen;

// Constructor
Address::Address()
    : m_lastname(new char[1]),
      m_firstname(new char[1]),
      m_address(new char[1]),
      m_phone(new char[1])
{
    // Initialise all strings to empty
    m_lastname[0] = m_firstname[0] = m_phone[0] = m_address[0] = '\0';
}

// Destructor
Address::~Address()
{
    // Clean up memory
    delete[] m_lastname;
    delete[] m_firstname;
    delete[] m_phone;
    delete[] m_address;
}

char* Address::dup(const char* source)
{
    // Allocate space for string, including NULL terminator
    char* result = new char[strlen(source) + 1];

    // Copy contents into newly allocated string
    strcpy(result, source);

    return result;
}

Address::Address(const Address& a2)
    : m_lastname(0), m_firstname(0), m_address(0), m_phone(0)
{
    // Use assignment operator to do the hard work
    *this = a2;
}

const Address& Address::operator=(const Address& a2)
{
    if (this != &a2)
    {
        lastname(a2.m_lastname);
        firstname(a2.m_firstname);
        phone(a2.m_phone);
        address(a2.m_address);
    }

    return *this;
}

void Address::lastname(const char *source)
{
    if (m_lastname != source)
    {
        delete[] m_lastname;
        m_lastname = dup(source);
    }
}

void Address::firstname(const char *source)
{
    if (m_firstname != source)
    {
        delete[] m_firstname;
        m_firstname = dup(source);
    }
}

void Address::phone(const char *source)
{
    if (m_phone != source)
    {
        delete[] m_phone;
        m_phone = dup(source);
    }
}

void Address::address(const char *source)
{
    if (m_address != source)
    {
        delete[] m_address;
        m_address = dup(source);
    }
}
