#ifndef ADDRESS_H_INCLUDED
#define ADDRESS_H_INCLUDED

#include <string>

/// Address class implemented using std::string class
class Address
{
public:
    /// Default constructor initializes recordID to 0 and all strings to empty
    Address() : m_recordID(0) { }

    /* Automatically generated by the compiler:
       ~Address();
       Address(const Address&);
       Address& operator=(const Address&);
    */

    /// return m_recordID;
    int recordID() const
    {
        return m_recordID;
    }

    /// m_recordID = i;
    void recordID(int i)
    {
        m_recordID = i;
    }

    /// return m_lastname;
    std::string lastname() const
    {
        return m_lastname;
    }
    void lastname(const std::string&);

    /// return m_firstname;
    std::string firstname() const
    {
        return m_firstname;
    }
    void firstname(const std::string&);

    /// return m_phone;
    std::string phone() const
    {
        return m_phone;
    }
    void phone(const std::string&);

    /// return m_address;
    std::string address() const
    {
        return m_address;
    }
    void address(const std::string&);
private:
    // Data fields
    /// Record ID of the address
    int         m_recordID;
    /// Lastname of the address
    std::string m_lastname;
    /// Firstname of the address
    std::string m_firstname;
    /// Address of the address
    std::string m_address;
    /// Phone for the address
    std::string m_phone;
};

bool operator==(const Address&, const Address&);
bool operator< (const Address&, const Address&);

#include <utility>
using namespace std::rel_ops;

#endif // ADDRESS_H_INCLUDED
