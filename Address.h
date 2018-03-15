#ifndef ADDRESS_H_INCLUDED
#define ADDRESS_H_INCLUDED

class Address
{
public:
    // Constructor
    Address();

    const char * lastname() const { return m_lastname;}
    void lastname(const char *);

    const char * firstname() const { return m_lastname;}
    void firstname(const char *);

    const char * phone() const { return m_lastname;}
    void phone(const char *);

    const char * address() const { return m_lastname;}
    void address(const char *);
private:
    // Enumerate string lengths
    static const int namelen = 16;
    static const int phonelen = 16;
    static const int addrlen = 100;

    // Data Fields
    char m_lastname[namelen];
    char m_firstname[namelen];
    char m_phone[phonelen];
    char m_address[addrlen];
};

#endif // ADDRESS_H_INCLUDED
