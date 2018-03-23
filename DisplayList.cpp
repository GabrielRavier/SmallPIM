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
void DisplayList::fillCacheFwd(int start, int numNeeded)
{
    int startID = 0;
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
void DisplayList::fillCacheBwd(int start, int numNeeded)
{
    int startID = 0;
    if (m_cache.empty())
        // Start caching from end of list
        m_cachedLast = true;
    else
    {
        // Start retrieving from first item in cache
        assert(start < m_cache.size())
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
    int recsToShow = min(m_linesPerScreen, int(m_cache.size() - m_firstVisibleIDx));

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
