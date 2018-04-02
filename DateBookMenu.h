#ifndef DATEBOOKMENU_H
#define DATEBOOKMENU_H

#include "Menu.h"

#include <memory>

#include "AppointmentDisplayList.h"

// Forward reference
class DateBookMenuCatalog;

class DateBookMenu : public Menu
{
public:
    DateBookMenu(DateBook& dateBook, DateBookMenuCatalog* catalog) : m_dateBook(dateBook), m_currDate(DateTime::now()),
        m_catalog(catalog)
    {

    }

    void setDate(DateTime dt);

protected:
    void createEntry();
    void showDay();
    void showWeek();
    void showMonth();
    void search();
    void gotoDate();

    virtual void reset();

    DateBook&               m_dateBook;
    DateTime                m_currDate;
    DateBookMenuCatalog*    m_catalog;
};

/// Derived class for Monthly view
class MonthlyDateBookMenu : public DateBookMenu
{
public:
    MonthlyDateBookMenu(DateBook& dateBook, DateBookMenuCatalog* catalog) : DateBookMenu(dateBook, catalog), m_cacheGood(false)
    {

    }

    void mainLoop();

private:
    void displayMonth();
    void showNext();
    void showPrevious();

    virtual void reset();

    bool m_scoreBoard[32];  // Index zero unused
    /// True if scoreBoard has valid data
    bool m_cacheGood;
};

/// Based class for all list-based date book views
class ListBasedDateBookMenu : public DateBookMenu
{
public:
    ListBasedDateBookMenu(DateBook& dateBook, DateBookMenuCatalog* catalog) : DateBookMenu(dateBook, catalog),
        m_displayList(dateBook)
    {

    }

protected:
    void viewEntry();
    void editEntry();
    void deleteEntry();

    virtual void reset();

    AppointmentDisplayList m_displayList;
};

/// Derived class for Weekly view
class WeeklyDateBookMenu : public ListBasedDateBookMenu
{
public:
    WeeklyDateBookMenu(DateBook& dateBook, DateBookMenuCatalog* catalog);

    void mainLoop();

private:
    void showNext();
    void showPrevious();
};

/// Derived class for Daily view
class DailyDateBookMenu : public ListBasedDateBookMenu
{
public:
    DailyDateBookMenu(DateBook& dateBook, DateBookMenuCatalog* catalog);

    void mainLoop();

private:
    void showNext();
    void showPrevious();
};

/// Derived class for string search view
class StringSearchDateBookMenu : public ListBasedDateBookMenu
{
public:
    StringSearchDateBookMenu(DateBook& dateBook, DateBookMenuCatalog* catalog);

    std::string searchString() const
    {
        return m_searchString;
    }

    void searchString(const std::string& s);

    void mainLoop();

private:
    void showPrevious();
    void showNext();
    std::string m_searchString;
};

/// Class to collect all of the date book menus
class DateBookMenuCatalog
{
public:
    DateBookMenuCatalog(DateBook& dateBook);

    // Use compiler-generated destructor
    // ~DateBookMenuCatalog();

    DailyDateBookMenu* dailyMenu();
    WeeklyDateBookMenu* weeklyMenu();
    MonthlyDateBookMenu* monthlyMenu();
    StringSearchDateBookMenu* stringSearchMenu();

private:
    // Copying would be very destructive, so disable it.
    DateBookMenuCatalog(const DateBookMenuCatalog&);
    DateBookMenuCatalog& operator=(const DateBookMenuCatalog&);

    // This object "owns" a copy of each menu type
    std::auto_ptr<DailyDateBookMenu> m_dailyMenu;
    std::auto_ptr<WeeklyDateBookMenu> m_weeklyMenu;
    std::auto_ptr<MonthlyDateBookMenu> m_monthlyMenu;
    std::auto_ptr<StringSearchDateBookMenu> m_stringSearchMenu;
};
#endif // DATEBOOKMENU_H
