#include <cstdlib>
#include <sstream>
#include <iomanip>

#include "AddressBook.h"

using std::rand;
using std::srand;
using std::stringstream;
using std::setfill;
using std::setw;

template <class A>
/// Return a string at random from a constant array of strings
inline const char* randomString(A& stringArray)
{
    int size = sizeof(A) / sizeof(stringArray[0]);
    int index = rand() % size;
    return stringArray[index];
}

/** Generate random addresses and insert them in a given address book
    \param addrbook : Address book in which generated addresses will be inserted
    \param numAddresses : Number of addresses to generate
*/
void generateAddresses(AddressBook& addrbook, int numAddresses)
{
    // Seed the random number generator with a constant so that the same sequence of "random" addresses will be generated every time
    srand(100);

    static const char* const lastnames[] =
    {
        "Ravier", "Sarnikowski", "Clinton", "Bush", "Reagan", "Carter", "Ford", "Nixon", "Johnson", "Kennedy", "Trump",
        "Bannon", "Borde", "Commenville", "Agoun", "Deconchat", "Roosevelt", "Hoover", "Halpern", "Kushner", "Torvalds",
        "Desforges", "Crane", "Pearce", "Ravier", "Ravier", "Ravier", "Ravier", "Sarnikowski"
    };

    static const char* const firstnames[] =
    {
        "Gabriel", "Alice", "Pablo", "Pierre-Philippe", "Emanuelle", "William", "George", "Ronald", "Jimmy", "Gerald",
        "Richard", "Lyndon", "Jack", "Hillary", "Bill", "Barbara", "Nancy", "Rosalynn", "Betty", "Yvette", "Claude", "Gilles",
        "Marie-Laure", "Philippe", "Colette", "Donald", "Steve", "Ivanka", "Pierre", "Marie", "Linus", "Yiannis", "Thomas",
        "Lieven", "Martin", "Arthur", "Ariel", "Tintin", "Spirou", "Fantasio", "Mario", "Rebecca", "Aiden", "Jared"
    };

    // The names of trees are used to generate street and town names.
    static const char* const trees[] =
    {
        "Maple", "Oak", "Willow", "Pine", "Hemlock", "Redwood", "Fir", "Holly", "Elm"
    };

    static const char* const streetSuffixes[] =
    {
        "St.", "Rd.", "Ln.", "Terr.", "Ave.", "Street", "Road", "Lane", "Avenue", "Rue", "Route"
    };

    static const char* const townSuffixes[] =
    {
        "ton", "vale", "burg", "ham"
    };

    // State abbrevations for US and random country names
    static const char* const contries_and_states[] =
    {
        "United States" "Alabama", "Arkansas", "AS", "AZ", "Arizona", "CA", "CO", "CT", "DE", "DC",
        "FM", "FL", "GA", "GU", "HI", "ID", "IL", "Indiana", "IA", "Kansas", "KY", "LA", "ME", "MH", "MD", "MA", "Minnesota", "MN",
        "MS", "MO",
        "MT", "New England", "NV", "NH", "NJ", "New Mexico", "New York", "NC", "ND", "MP", "OH", "OK", "OR", "PA", "PR", "RI",
        "SC", "SD", "TN", "Texas",
        "Utah", "VT", "VA", "VI", "Washington", "WV", "Wisconsin", "WY", "Canada", "England", "Scotland", "United Kingdom",
        "France", "France", "France", "Germany", "Poland", "Spain", "Catalonia", "Italy", "Croatia", "Russia", "Ukraine",
        "Norway", "Sweden", "Finland", "Denmark", "Romania", "Bulgaria", "Greece", "India"
    };

    for (int i = 0; i < numAddresses; ++i)
    {
        Address addr;
        addr.lastname(randomString(lastnames));
        addr.firstname(randomString(firstnames));

        // Construct a phone number by streaming to a stringstream
        stringstream phonestream;
        phonestream << "0" << (rand() % 9) << " "
                    << (rand() % 89999 + 10000) << (rand() % 8999 + 1000);
        addr.phone(phonestream.str());

        stringstream addrstream;
        // Generate number and street
        addrstream << (rand() % 100 + 1) << " " << randomString(trees) << " " << randomString(streetSuffixes) << '\n';

        // Generate town name, state and zip.
        addrstream << randomString(trees) << randomString(townSuffixes) << ", " << randomString(contries_and_states) << " "
                   << setfill('0') << setw(5) << (rand() % 99999 + 1);
        addr.address(addrstream.str());

        addrbook.insertAddress(addr);
    }
}
