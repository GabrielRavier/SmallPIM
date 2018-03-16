#ifndef ADDRESS_H_INCLUDED
#define ADDRESS_H_INCLUDED

// Address class using dynamically allocated strings
class Address
{
public:
    // Constructor
    Address();

    // Destructor
    ~Address();

    // Copy constructor and assignment
    Address(const Address&);
    const Address& operator=(const Address&);

    const char * lastname() const { return m_lastname;}
    void lastname(const char *);

    const char * firstname() const { return m_firstname;}
    void firstname(const char *);

    const char * phone() const { return m_phone;}
    void phone(const char *);

    const char * address() const { return m_address;}
    void address(const char *);
private:
    // Variable-length data fields
    char* m_lastname;
    char* m_firstname;
    char* m_address;
    char* m_phone;

    // Private function for making a copy of a string:
    char* dup(const char* source);
};

#endif // ADDRESS_H_INCLUDED
