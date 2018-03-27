#include "DisplayList.h"

#include <cassert>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>

using std::vector;
using std::copy;
using std::back_inserter;
using std::front_inserter;
using std::cout;
using std::endl;
using std::deque;
using std::setw;
using std::setfill;
using std::right;
using std::dec;
using std::min;
using std::max;
using std::find;
using std::cin;


// Constructor sets screen size
DisplayList::DisplayList(int linesPerScreen)
    : m_linesPerScreen(linesPerScreen)
{
    reset();
}

// Destructor doesn't have to do much
DisplayList::~DisplayList() { }

// Clear all data
void DisplayList::reset()
{
    m_cache.clear();
    m_cachedFirst = false;
    m_cachedLast = false;
    m_firstVisibleIDx = 0;
}

/* Fill cache in the forward direction. Specify start index and number of desired records.
    If not enough records are available, will set m_cachedLast. */
void DisplayList::fillCacheFwd(unsigned int start, int numNeeded)
{
    unsigned int startID = 0;
    if (m_cache.empty())
        // Start caching from beginning of list
        m_cachedFirst = true;
    else
    {
        // Start retrieving from last item in cache
        assert(start < m_cache.size());
        startID = m_cache.back();
    }

    int recordTillEnd = m_cache.size() - start;
    if (! m_cachedLast && recordTillEnd < numNeeded)
    {
        // Too few entries in cache to fill screen. Need to fetch more.

        vector<int> moreRecords;
        m_cachedLast = fetchMore(startID, numNeeded - recordTillEnd, moreRecords);

        copy(moreRecords.begin(), moreRecords.end(), back_inserter(m_cache));
    }
}

/* Fill cache in the backward direction. Specify start index and number of desired records.
   If not enough records are available, will set m_cachedFirst. */
void DisplayList::fillCacheBwd(unsigned int start, int numNeeded)
{
    int startID = 0;
    if (m_cache.empty())
        // Start caching from end of list
        m_cachedLast = true;
    else
    {
        // Start retrieving from first item in cache
        assert(start < m_cache.size());
        startID = m_cache.front();
    }

    int recordsTillStart = start;
    if (! m_cachedFirst && recordsTillStart < numNeeded)
    {
        // Too few entries in cache to fill screen. Need to fetch more.

        vector<int> moreRecords;
        m_cachedFirst = fetchMore(startID, -(numNeeded - recordsTillStart), moreRecords);

        copy(moreRecords.rbegin(), moreRecords.rend(), front_inserter(m_cache));

        // We inserted records before the first visible one. We must update m_firstVisibleIDx to reflect it's new position.
        m_firstVisibleIDx += moreRecords.size();
    }
}

void DisplayList::display()
{
    // Make sure cache contains a screenful of records
    fillCacheFwd(m_firstVisibleIDx, m_linesPerScreen);

    // If after attempting to fill cache, it is still empty, then there are no records to display
    if (m_cache.empty())
    {
        // Display empty-list marker
        cout << "=============== No records selected ===============" << endl;
        return;
    }

    /* Calculate the number of records to display. It is the smaller of the number of lines in a screenful
       or the number of records left at the end of the cache. */
    int recsToShow = min((int)m_linesPerScreen, int(m_cache.size() - m_firstVisibleIDx));

    if (atStart())
        // Display start-of-list marker.
        cout << "=============== Start of list ===============\n";

    deque<int>::iterator start = m_cache.begin() + m_firstVisibleIDx;
    deque<int>::iterator finish = start + recsToShow;
    for (deque<int>::iterator i = start; i != finish; ++i)
    {
        // Display line number
        int lineNum = i - start + 1;    // Start counting at 1
        cout << setw(2) << setfill(' ') << right << dec << lineNum << ": ";
        displayRecord(*i);
        cout << endl;
    }

    if (atEnd())
        // Display end-of-list marker.
        cout << "=============== End of list ===============";

    cout << endl;
}

void DisplayList::pageDown()
{
    // Scroll current bottom-of-screen line to top of screen
    if (atEnd())
        return;

    // Cache current screenful and next screenful
    fillCacheFwd(m_firstVisibleIDx, 2 * m_linesPerScreen);

    // Advance visible index one screenful, but only if there exists at least one screenful past the current m_firstVisibleIDx.
    if (! atEnd())
        m_firstVisibleIDx += m_linesPerScreen;
}

void DisplayList::pageUp()
{
    // Scroll current top-of-screen line to bottom of screen
    if (atStart())
        return;

    // Cache previous screenful
    fillCacheBwd(m_firstVisibleIDx, m_linesPerScreen);

    // Advance visible index backward one screenful, but not past start of cache.
    m_firstVisibleIDx = max((int)(m_firstVisibleIDx - m_linesPerScreen), 0);
}

void DisplayList::toStart()
{
    if (m_cachedFirst)
        m_firstVisibleIDx = 0;
    else
        // Cached records do not include top of cache.
        reset();
}

bool DisplayList::atStart()
{
    return m_cachedFirst && (m_firstVisibleIDx == 0);
}

bool DisplayList::atEnd()
{
    return (m_cachedLast && (m_cache.size() - m_firstVisibleIDx <= m_linesPerScreen));
}

// Scroll so that specified record is at top of the screen.
void DisplayList::scrollToTop(int recordID)
{
    assert(recordID != 0);

    // Find specified record in cache:
    cache_t::iterator found = find(m_cache.begin(), m_cache.end(), recordID);

    // If didn't find record in cache, flush cache and start reloading.
    if (found == m_cache.end())
    {
        reset();
        m_cache.push_back(recordID);
        m_firstVisibleIDx = 0;
    }
    else
        m_firstVisibleIDx = found - m_cache.begin();

    fillCacheFwd(m_firstVisibleIDx, m_linesPerScreen);
}

// Return ID of nth record on screen
int DisplayList::screenRecord(int n) const
{
    if (m_firstVisibleIDx + n >= m_cache.size())
        return 0;
    else
        return m_cache[m_firstVisibleIDx + n];
}

int DisplayList::selectRecord()
{
    while (cin.good())
    {
        unsigned maxSelection = min(int(m_cache.size() - m_firstVisibleIDx), (int)m_linesPerScreen);

        if (!maxSelection)
        {
            cout << "No records to select\n";
            return 0;
        }

        // Prompt for record number in visible range
        cout << "Choose a record number between " << 1 << " and " << maxSelection << "\nRecord number (0 to cancel)? ";

        unsigned selection = 0;

        cin >> selection;
        if (cin.fail())
        {
            if (cin.bad() || cin.eof())
                break;

            /* Recoverable input error. (User typed a non-numeric input).
               Clear error condition and throw away rest of input line then continue loop */
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Invalid selection, please try again.\n\n";
            continue;
        }

        // Throw away rest of input line
        cin.ignore(INT_MAX, '\n');

        if (selection == 0)
            return 0;

        // Make sure user chose one of the visible records.
        if (1 <= selection && selection <= maxSelection)
            return m_cache[m_firstVisibleIDx + selection - 1];
        else
            cout << "Invalid selection, please try again.\n\n";

    }

    return 0;
}
