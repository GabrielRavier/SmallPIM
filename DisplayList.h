#ifndef DISPLAYLIST_H
#define DISPLAYLIST_H

#include <deque>
#include <vector>

/* This is an abstract class used to display a list of records where the specific record being displayed is determined
   by the derived classes. If there are more records in the list than can fit on a screen, a partial list is displayed
   and functions are provided for scrolling up and down in the list. */

class DisplayList
{
public:
    DisplayList(int linesPerScreen = 15);
    virtual ~DisplayList();

    void display();     // Display the list
    void pageDown();    // Scroll down one screenful
    void pageUp();      // Scroll up one screenful
    void toStart();     // Scroll up to the first screenful
    bool atStart();     // True if displaying first screenful
    bool atEnd();       // True if displaying last screenful
    void reset();       // Reinitialize (discard cached records)
    int screenRecord(int n) const;  // Return ID of nth record on screen

    // Scroll so that specified record is at top of the screen.
    void scrollToTop(int recordID);

    // Ask the user for a record number and return selected recordID. Return 0 if no record selected (e.g. user aborted)
    int selectRecord();

protected:
    // Derived classes define displayRecord to display a particular record type
    virtual void displayRecord(int recordID) = 0;

    /* Derived classes define fetchMore to get numRecords more records starting at, but not including startID.
       If numRecords is negative, then retrieve -numRecords records BEFORE startID. If startID is zero, then
       return the records BEFORE startID. If startID is zero, then return the first (or last) records.
       Result may have more records than requested. Returns true if the last (or first) record is in the result. */
    virtual bool fetchMore(int startID, int numRecords, std::vector<int>& result) = 0;

private:

    typedef std::deque<int> cache_t;    // Type of record ID cache

    int     m_linesPerScreen;   // Number of lines per screenful
    cache_t m_cache;            // Cache of known record IDs.
    bool    m_cachedFirst;      // True if m_cache contains first record
    bool    m_cachedLast;       // True if m_cache contains last record
    int     m_firstVisibleIDx;  // Deque index of first visible record

    /* Fill cache in the forward direction. Specify start index and number of desired records.
       If not enough records are available, will set m_cachedLast. */
    void fillCacheFwd(int start, int numNeeded);

    /* Fill cache in the backward direction. Specify start index and number of desired records.
       If not enough records are available, will set m_cachedFirst. */
    void fillCacheBwd(int start, int numNeeded);
};

#endif // DISPLAYLIST_H
