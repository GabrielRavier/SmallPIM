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
        "Bannon", "Borde", "Commenville", "Agoun", "Deconchat", "Roosevelt", "Hoover", "Halpern"
    };

    static const char* const firstnames[] =
    {

    };
}
