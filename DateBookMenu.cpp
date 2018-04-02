#include "DateBookMenu.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <climits>
#include <utility>

#include "Appointment.h"
#include "DateBook.h"
#include "AppointmentEditor.h"

using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::getline;
using std::cin;
using std::ios;

void DateBookMenu::setDate(DateTime dt)
{
    m_currDate = dt;
    reset();
}

void DateBookMenu::reset()
{

}

void DateBookMenu::createEntry()
{
    // Edit an empty appointment
    Appointment appt;
    appt.startTime(m_currDate);
    appt.endTime(m_currDate);
    AppointmentEditor editor(appt);

    // Continue editing until a record is saved or canceled.
    while (editor.edit())
    {
        appt = editor.appt();
        if (appt.description().empty())
        {
            cout << "Description must not be empty." << endl;
            continue;   // Loop and re-edit
        }

        // Insert record.
        m_dateBook.insertAppointment(appt);
        setDate(appt.startTime());
        reset();
        break;
    } // End while
}

void DateBookMenu::showDay()
{
    // Go to daily mode for the current date
    exitMenu();
    m_catalog->dailyMenu()->setDate(m_currDate);
    enterMenu(m_catalog->dailyMenu());
}

void DateBookMenu::showWeek()
{
    // Go to weekly mode for the current date
    exitMenu();
    m_catalog->weeklyMenu()->setDate(m_currDate);
    enterMenu(m_catalog->weeklyMenu());
}

void DateBookMenu::showMonth()
{
    // Go to monthly mode for the current date
    exitMenu();
    m_catalog->monthlyMenu()->setDate(m_currDate);
    enterMenu(m_catalog->monthlyMenu());
}

void DateBookMenu::search()
{
    string searchString;
    cout << "Search for string : ";
    getline(cin, searchString);

    // If no search string entered, abort.
    if (cin.fail() || searchString.empty())
        return;

    // Set both search string and date
    exitMenu();
    m_catalog->stringSearchMenu()->searchString(searchString);
    m_catalog->stringSearchMenu()->setDate(m_currDate);
    enterMenu(m_catalog->stringSearchMenu());
}

void DateBookMenu::gotoDate()
{
    string dateString;
    while (cin.good())
    {
        cout << "Goto date [" << m_currDate.dateStr() << "] : ";
        getline(cin, dateString);
        if (dateString.empty())
            break;
        else if (dateString[0] == 't' || dateString[0] == 'T')
        {
            // User entered "today"
            setDate(DateTime::now());
            break;
        }

        // Set date value
        DateTime newDate;
        if (!newDate.dateStr(dateString))
            cout << "Invalid date, please try again\n";
        else
        {
            setDate(newDate);
            break;
        }
    } // End while
}

void ListBasedDateBookMenu::reset()
{
    m_displayList.reset();
    m_displayList.currDate(m_currDate);
}

void ListBasedDateBookMenu::viewEntry()
{
    int recordID = m_displayList.selectRecord();
    if (recordID <= 0)
        return;

    setDate(m_dateBook.getAppointment(recordID).startTime());

    Appointment appt = m_dateBook.getAppointment(recordID);
    cout << "Date: " << appt.startTime().dateStr() << '\n'
         << "From " << appt.startTime().timeStr() << " to " << appt.endTime().timeStr() << '\n'
         << appt.description()
         << "\n\nPress [RETURN] when ready.";
    cin.ignore(INT_MAX, '\n');
}

void ListBasedDateBookMenu::deleteEntry()
{
    int recordID = m_displayList.selectRecord();
    if (recordID <= 0)
        return;

    setDate(m_dateBook.getAppointment(recordID).startTime());

    // Erase the appointment
    m_dateBook.eraseAppointment(recordID);

    // Deleting the entry invalidates the display list cache. Reset it, then scroll back to the previous location
    m_displayList.reset();
}

void ListBasedDateBookMenu::editEntry()
{
    int recordID = m_displayList.selectRecord();
    if (recordID <= 0)
        return;

    // Create an editor for the selected appointment
    Appointment appt = m_dateBook.getAppointment(recordID);
    AppointmentEditor editor(appt);

    // Edit the appointment
    if (editor.edit())
    {
        // Replace appointment with modified version
        m_dateBook.replaceAppointment(editor.appt());

        // Appointment's sort order might have changed. We need to reset the display list.
        m_displayList.reset();
        setDate(editor.appt().startTime());
    }
}

DateBookMenuCatalog::DateBookMenuCatalog(DateBook& dateBook) : m_dailyMenu(new DailyDateBookMenu(dateBook, this)),
    m_weeklyMenu(new WeeklyDateBookMenu(dateBook, this)),
    m_monthlyMenu(new MonthlyDateBookMenu(dateBook, this)),
    m_stringSearchMenu(new StringSearchDateBookMenu(dateBook, this))
{

}

DailyDateBookMenu* DateBookMenuCatalog::dailyMenu()
{
    return m_dailyMenu.get();
}

WeeklyDateBookMenu* DateBookMenuCatalog::weeklyMenu()
{
    return m_weeklyMenu.get();
}

MonthlyDateBookMenu* DateBookMenuCatalog::monthlyMenu()
{
    return m_monthlyMenu.get();
}

StringSearchDateBookMenu* DateBookMenuCatalog::stringSearchMenu()
{
    return m_stringSearchMenu.get();
}

void MonthlyDateBookMenu::mainLoop()
{
    clearScreen();
    int year, month, day;
    m_currDate.getDate(year, month, day);
    cout << "*** Appointment Book ***\n" << "Month of " << m_currDate.monthName() << ' ' << year << "\n\n";

    displayMonth();
    cout << '\n';

    const char menu[] = "(P)revious month, (N)ext month, (C)reate, (S)earch,\n"
                        "(R)edisplay, (D)aily view, (W)eekly view, (G)oto date, "
                        "(Q)uit ? ";
    const char choices[] = "PNCSRDWGQ";

    switch (getMenuSelection(menu, choices))
    {
    case 'P':
        showPrevious();
        break;
    case 'N':
        showNext();
        break;
    case 'C':
        createEntry();
        break;
    case 'S':
        search();
        break;
    case 'R':
        // Do nothing, just loop
        break;
    case 'D':
        showDay();
        break;
    case 'W':
        showWeek();
        break;
    case 'G':
        gotoDate();
        break;
    case 'Q':
    default:
        exitMenu();
        break;
    }
}

void MonthlyDateBookMenu::reset()
{
    m_cacheGood = false;
}

void MonthlyDateBookMenu::displayMonth()
{
    using namespace std::rel_ops;

    // Calculate start of current month and next month
    DateTime startOfMonth = m_currDate.startOfMonth();
    DateTime nextMonth = startOfMonth.addDay(31).startOfMonth();
    int year, month, day;

    if (! m_cacheGood)
    {
        // Clear the scoreboard
        for (int i = 0; i < 32; ++i)
            m_scoreBoard[i] = false;

        // Iterate through every entry within the month
        DateBook::const_iterator iter = m_dateBook.findAppointmentAtTime(startOfMonth);
        DateBook::const_iterator endIter = m_dateBook.findAppointmentAtTime(nextMonth);

        // For each entry, set the scoreboard value for that day to true
        for ( ; iter != endIter; ++ iter)
        {
            iter->startTime().getDate(year, month, day);
            m_scoreBoard[day] = true;
        }
    }

    // Calculate the day of the week for the first day of the month.
    int startWday = startOfMonth.dayOfWeek();

    // Calculate last day of the month (== day zero of next month)
    nextMonth.addDay(-1).getDate(year, month, day);
    int monthLength = day;

    cout << ' ';

    int wday;
    // Print blank spaces for first few day of first week of month.
    for (wday = 0; wday < startWday; ++wday)
        cout << "   ";

    // Print each day of month
    cout.fill(' ');
    cout.setf(ios::dec, ios::basefield);
    cout.setf(ios::right, ios::adjustfield);
    for (int mday = 1; mday <= monthLength; ++mday)
    {
        if (!wday && mday != 1)
            cout << "\n ";
        cout << setw(3) << mday << (m_scoreBoard[mday] ? '*' : ' ');
        wday = (wday + 1) % 7;
    }
    cout << endl;
}

void MonthlyDateBookMenu::showPrevious()
{
    // Subtract one month
    int year, month, day, hour, min;
    m_currDate.get(year, month, day, hour, min);
    setDate(DateTime(year, month - 1, day, hour, min));
}

void MonthlyDateBookMenu::showNext()
{
    // Add one month
    int year, month, day, hour, min;
    m_currDate.get(year, month, day, hour, min);
    setDate(DateTime(year, month + 1, day, hour, min));
}

WeeklyDateBookMenu::WeeklyDateBookMenu(DateBook& dateBook, DateBookMenuCatalog* catalog)
    : ListBasedDateBookMenu(dateBook, catalog)
{
    m_displayList.listWeek(DateTime::now());
}

void WeeklyDateBookMenu::mainLoop()
{
    clearScreen();
    cout << "*** Appointment Book ***\n"
         << "Week of " << m_currDate.startOfWeek().dateStr() << " to " << m_currDate.startOfWeek().addDay(6).dateStr() << "\n\n";

    m_displayList.display();
    cout << '\n';

    static const char menu[] = "(P)revious week, (N)ext week,\n"
    "scroll (B)ackward, scroll (F)orward, (V)iew,\n"
    "(C)reate, (D)elete, (E)dit, (S)earch, (R)edisplay,\n"
    "d(A)ily view, (M)onthly view, (G)oto date, (Q)uit ? ";
    static const char choices[] = "PNBFVCDESRAMGQ";

    switch (getMenuSelection(menu, choices))
    {
    case 'P':
        showPrevious();
        break;
    case 'N':
        showNext();
        break;
    case 'B':
        m_displayList.pageUp();
        break;
    case 'F':
        m_displayList.pageDown();
        break;
    case 'V':
        viewEntry();
        break;
    case 'C':
        createEntry();
        break;
    case 'D':
        deleteEntry();
        break;
    case 'E':
        editEntry();
        break;
    case 'S':
        search();
        break;
    case 'R':
        // Do nothing, just loop
        break;
    case 'A':
        showDay();
        break;
    case 'M':
        showMonth();
        break;
    case 'G':
        gotoDate();
        break;
    case 'Q':
    default:
        exitMenu();
        break;
    }
}

void WeeklyDateBookMenu::showPrevious()
{
    setDate(m_currDate.addDay(-7));
}

void WeeklyDateBookMenu::showNext()
{
    setDate(m_currDate.addDay(7));
}

DailyDateBookMenu::DailyDateBookMenu(DateBook& datebook, DateBookMenuCatalog* catalog)
    : ListBasedDateBookMenu(datebook, catalog)
{
    m_displayList.listDay(DateTime::now());
}

void DailyDateBookMenu::mainLoop()
{
    clearScreen();
    cout << "*** Appointment Book ***\n"
         << m_currDate.wdayName() << ' ' << m_currDate.dateStr() << "\n\n";

    m_displayList.display();
    cout << '\n';

    static const char menu[] = "(P)revious day, (N)ext day,\n"
    "scroll (B)ackwards, scroll (F)orward, (V)iew,\n"
    "(C)reate, (D)elete, (E)dit, (S)earch, (R)edisplay,\n"
    "(W)eekly view, (M)onthly view, (G)oto date, (Q)uit ? ";
    static const char choices[] = "PNBFVCDESRWMGQ";

    switch (getMenuSelection(menu, choices))
    {
    case 'P':
        showPrevious();
        break;
    case 'N':
        showNext();
        break;
    case 'B':
        m_displayList.pageUp();
        break;
    case 'F':
        m_displayList.pageDown();
        break;
    case 'V':
        viewEntry();
        break;
    case 'C':
        createEntry();
        break;
    case 'D':
        deleteEntry();
        break;
    case 'E':
        editEntry();
        break;
    case 'S':
        search();
        break;
    case 'R':
        // Do nothing, just loop
        break;
    case 'W':
        showWeek();
        break;
    case 'M':
        showMonth();
        break;
    case 'G':
        gotoDate();
        break;
    case 'Q':
    default:
        exitMenu();
        break;
    }
}

void DailyDateBookMenu::showPrevious()
{
    setDate(m_currDate.addDay(-1));
}

void DailyDateBookMenu::showNext()
{
    setDate(m_currDate.addDay(1));
}

StringSearchDateBookMenu::StringSearchDateBookMenu(DateBook& dateBook, DateBookMenuCatalog* catalog)
    : ListBasedDateBookMenu(dateBook, catalog)
{
    m_displayList.listContainsString("");
}

void StringSearchDateBookMenu::mainLoop()
{
    clearScreen();
    cout << "*** Appointment Book ***\n"
         << "Records matching \"" << m_searchString << "\"\n\n";

    m_displayList.display();
    cout << '\n';

    static const char menu[] = "scroll (B)ackwards, scroll (F)orward, (V)iew,\n"
    "(C)reate, (D)elete, (E)dit, (S)earch, (R)edisplay,\n"
    "d(A)ily view, (W)eekly view, (M)onthly view, (G)oto date,"
    "(Q)uit ? ";
    static const char choices[] = "BFVCDESRAWMGQ";

    switch (getMenuSelection(menu, choices))
    {
    case 'B':
        m_displayList.pageUp();
        break;
    case 'F':
        m_displayList.pageDown();
        break;
    case 'V':
        viewEntry();
        break;
    case 'C':
        createEntry();
        break;
    case 'D':
        deleteEntry();
        break;
    case 'E':
        editEntry();
        break;
    case 'S':
        search();
        break;
    case 'R':
        // Do nothing, just loop
        break;
    case 'A':
        showDay();
        break;
    case 'W':
        showWeek();
        break;
    case 'M':
        showMonth();
        break;
    case 'G':
        gotoDate();
        showDay();
        break;
    case 'Q':
    default:
        exitMenu();
        break;
    }
}

void StringSearchDateBookMenu::searchString(const std::string& s)
{
    m_searchString = s;
    m_displayList.listContainsString(s);
}
