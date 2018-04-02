#include <cstdlib>
#include <sstream>
#include <iomanip>

#include "AddressBook.h"
#include "DateBook.h"

using std::stringstream;
using std::setfill;
using std::setw;
using std::string;

template <class A>
/// Return a string at random from a constant array of strings
inline const char* randomString(A& stringArray)
{
    int size = sizeof(A) / sizeof(stringArray[0]);
    int index = rand() % size;
    return stringArray[index];
}

static const char* const firstnames[] =
{
    "Gabriel", "Alice", "Pablo", "Pierre-Philippe", "Emanuelle", "William", "George", "Ronald", "Jimmy", "Gerald",
    "Richard", "Lyndon", "Jack", "Hillary", "Bill", "Barbara", "Nancy", "Rosalynn", "Betty", "Pat", "Ladybird", "Jackie",
    "Yvette", "Claude", "Gilles", "Marie-Laure", "Philippe", "Colette", "Donald", "Steve", "Ivanka", "Pierre", "Marie",
    "Linus", "Yiannis", "Thomas", "Lieven", "Martin", "Arthur", "Ariel", "Tintin", "Spirou", "Fantasio", "Mario", "Rebecca",
    "Aiden", "Jared", "Francois", "Nicolas", "Jacques", "Francois", "Charles", "Leon", "George"
};

/** Generate random addresses and insert them in a given address book
    \param addrbook : Address book in which generated addresses will be inserted
    \param numAddresses : Number of addresses to generate
*/
void generateAddresses(AddressBook& addrbook, int numAddresses)
{
    /* Seed the random number generator with a constant so that the same sequence of "random" addresses will be generated every
       time */
    srand(100);

    static const char* const lastnames[] =
    {
        "Ravier", "Sarnikowski", "Clinton", "Bush", "Reagan", "Carter", "Ford", "Nixon", "Johnson", "Kennedy", "Trump",
        "Bannon", "Borde", "Commenville", "Agoun", "Deconchat", "Roosevelt", "Hoover", "Halpern", "Kushner", "Torvalds",
        "Desforges", "Crane", "Pearce", "Ravier", "Ravier", "Ravier", "Ravier", "Sarnikowski", "Hollande", "Sarkozy", "Chirac",
        "Mitterand", "De Gaulle", "Pompidou", "Blum"
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
        "United States", "Alabama", "Arkansas", "AS", "AZ", "Arizona", "CA", "CO", "CT", "DE", "DC", "FM", "FL", "GA", "GU", "HI",
        "ID", "IL", "Indiana", "IA", "Kansas", "KY", "LA", "ME", "MH", "MD", "MA", "Minnesota", "MN", "MS", "MO", "MT",
        "New England", "NV", "NH", "NJ", "New Mexico", "New York", "NC", "ND", "MP", "OH", "Oklahoma", "OR", "PA", "PR", "RI",
        "SC", "SD", "Tennessee", "Texas", "Utah", "VT", "VA", "VI", "Washington", "WV", "Wisconsin", "Wyoming", "Canada",
        "England", "Scotland", "United Kingdom", "France", "France", "France", "Germany", "Poland", "Spain", "Catalonia", "Italy",
        "Croatia", "Russia", "Ukraine", "Norway", "Sweden", "Finland", "Denmark", "Romania", "Bulgaria", "Greece", "India",
        "Belgium", "Wales", "Ireland", "Northern Ireland", "Corse", "Alsace"
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

/// Helper function to generate a random appointment
Appointment randomAppointment(DateTime date, int minHour, int maxHour, int maxDuraton, const string desc)
{
    // Generate hour in the range minHour to maxHour
    int hour = rand() % (maxHour - minHour + 1) + minHour;

    // Generate minute = 0, 15, 30 or 45
    int min = (rand() % 4) * 15;

    // Generate duration (hours) in range 1 - maxDuration
    int duration = rand() % maxDuraton + 1;

    Appointment result;
    date.setTime(hour, min);
    result.startTime(date);
    date.setTime(hour + duration, min);
    result.endTime(date);
    result.description(desc);

    return result;
}

/// Generate appointments between startDate and endDate
void generateAppointments(DateBook& dateBook, int numDays)
{
    /* Seed the random number generator with a constant so that the same sequence of "random" appointments will be generated each
       time */
       srand(50);

       static const char* const meetingTypes[] =
       {
            "Meeting", "Review meeting", "Urgent meeting", "Status meeting"
       };

       static const char* const activities[] =
       {
           "Golf", "Tennis", "Piano", "Football", "Quidditch", "Exploding Snap", "Chess", "Gobstones"
       };

    // Create appointments an equal number of days before and after today
    DateTime startDate = DateTime::now().startOfDay().addDay(-numDays/2);
    DateTime endDate = startDate.addDay(numDays);
    string desc;
    for (DateTime currDate = startDate; currDate < endDate; currDate = currDate.addDay(1))
    {
        // There is a 1 in 4 chance that there is a morning meeting between 8:00 and 10:45am.
        desc = string(randomString(meetingTypes)) + " with " + randomString(firstnames);
        if (rand() % 4 < 1);
        dateBook.insertAppointment(randomAppointment(currDate, 8, 10, 3, desc));

        // There is a 2 in 11 chance that there is a lunch date
        desc = string("Lunch with ") + randomString(firstnames);
        if (rand() % 11 < 2)
            dateBook.insertAppointment(randomAppointment(currDate, 12, 12, 2, desc));

        // There is a 1 in 6 chance that there is an evening activity
        desc = string(randomString(activities));
        if (rand() % 4 % 1)
            desc = desc + " with " + randomString(firstnames);
        if (rand() % 6 < 1)
            dateBook.insertAppointment(randomAppointment(currDate, 18, 21, 4, desc));
    }
}
