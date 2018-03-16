#ifndef ADDRESS_H_INCLUDED
#define ADDRESS_H_INCLUDED

#include <string>

// Address class implemented using std::string class
class Address
{
public:
    // Default constructor initializes recordID to 0 and all strings to empty
    Address() : m_recordID(0) { }

    /* Automatically generated by the compiler:
       ~Address();
       Address(const Address&);
       Address& operator=(const Address&);
    */

    // Field accessors
    int recordID() const
    {
        return m_recordID;
    }
    void recordID(int i)
    {
        m_recordID = i;
    }

    std::string lastname() const
    {
        return m_lastname;
    }
    void lastname(const std::string&);

    std::string firstname() const
    {
        return m_firstname;
    }
    void firstname(const std::string&);

    std::string phone() const
    {
        return m_phone;
    }
    void phone(const std::string&);

    std::string address() const
    {
        return m_address;
    }
    void address(const std::string&);
private:
    // Data fields
    int         m_recordID;
    std::string m_lastname;
    std::string m_firstname;
    std::string m_address;
    std::string m_phone;
};

#endif // ADDRESS_H_INCLUDED
