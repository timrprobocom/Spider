#include <afxwin.h>

#include <stdio.h>
#include <stdlib.h>

#include "frame.h"
#include "spider.h"


class Card
{
public:	
    BYTE suit;		// 0..3
    BYTE rank;		// 0..12
    BYTE position;	// 0 == top card
    BOOL exposed;
    Card * fwd;
    Card * bkw;
};


Card deck [104];
Card * next;

Card * head [10];
Card * tail [10];

BYTE exposed [4] = { 1, 1, 1, 1 } ;	// 1 if a full suit is exposed

int deals = nDeals;
int SuitsRemoved = 0;

int highlight = -1;
int automove = 0;


// HEREIN should be encapsulated all of the platform-specific stuff.


#define pWnd    ((CMainWindow*)(AfxGetApp()->m_pMainWnd))


void
ClearColumn (CDC & dc, int col)
{
    pWnd->ClearColumnTail (dc, col, 0);
}


void
ClearColumnTail (CDC & dc, int col, int start)
{
    pWnd->ClearColumnTail (dc, col, start);
}


void
DisplayCard (CDC & dc, Card * c, int x)
{
    if (c->exposed)
	pWnd->PlaceBitmap (dc, c->suit, c->rank, x, c->position);
    else
	pWnd->PlaceBitmap (dc, 4, 0, x, c->position);
}



void
DisplayColumnNumber (CDC & dc, int col, BYTE intense = FALSE)
{
    pWnd->ColumnNumber (dc, col, intense);
}


void
DisplayError (char * s)
{
    pWnd->SetError (s);
}


void
UpdateDeals ()
{
    char str [80];

    wsprintf (str, "%d deals remaining.", deals);
    pWnd->SetDeals (str);

    if (SuitsRemoved) {
        wsprintf (str, "%d suits removed.", SuitsRemoved);
	pWnd->SetSuits (str);
    }
}


// END platform-specific stuff


void
RenumberColumn (int col)
{
    Card * cptr;
    BYTE ordinal = 0;

    for (
	cptr = head [col];
	cptr;
	cptr = cptr->fwd
    )
        cptr->position = ordinal++;
}


void
DisplayFullSuitMsg (int c)
{
    static char * suits [] = {
	"Clubs",
	"Diamonds",
	"Hearts",
	"Spades"
    };

    if (exposed [c]) {
	CString sz;
	sz = "A full suit of " + CString(suits [c]) + " is now exposed.";
	pWnd->MessageBox (sz);
	pWnd->ShowSuit (c);
    }
    else {
        pWnd->HideSuit (c);
    }
}


void
SetupDeck (Card * d)
{
    BYTE i;

    for (i = 0; i < 104; i++, d++)
    {
	d->suit = (BYTE) (i / 26);
	d->rank = (BYTE) (i % 13);
	d->exposed = 0;
	d->fwd = i == 103 ? 0 : d+1;
	d->bkw = i == 0 ? 0 : d-1;
    }
}


void
Shuffle (Card * d)
{
    int i;
    int a;
    int b;

    for (i = 0; i < 220; i++)
    {
	a = (int) ((((DWORD) rand ()) * 104) >> 15);
	b = (int) ((((DWORD) rand ()) * 104) >> 15);
	if (a != b)
	{
	    register BYTE tmps;
	    register BYTE tmpr;

	    tmps = d [a].suit;
	    tmpr = d [a].rank;
	    d [a].suit = d [b].suit;
	    d [a].rank = d [b].rank;
	    d [b].suit = tmps;
	    d [b].rank = tmpr;
	}
    }
}


Card *
Deal (Card * cptr)
{
    int i;
    static short size [] = { 6, 5, 5, 6, 5, 5, 6, 5, 5, 6 };

    for (i = 0; i < 10; i++)
    {
	head [i] = cptr;
	cptr->bkw = 0;
	cptr += size [i] - 1;
	cptr->fwd = 0;
	cptr->exposed = 1;
	tail [i] = cptr;
	cptr ++;

	RenumberColumn (i);
    }

    return cptr;
}


Card *
FindTopOfSuit (int col)
{
    BYTE s = tail [col]->suit;
    Card * cptr = tail [col];

    while (cptr->bkw &&
        cptr->bkw->suit == s &&
	cptr->bkw->exposed &&
	cptr->bkw->rank == cptr->rank + 1)
	cptr = cptr->bkw;

    return cptr;
}


void
CheckSuits ()
{
    BYTE ctrs [4];
    BYTE flgs [4][13];

    memset (ctrs, 0, 4);
    memset (flgs, 1, 4*13);

    for (int col = 0; col < 10; col++)
    {
        Card * cptr = head [col];
	while (cptr)
	{
	    if (cptr->exposed)
	    {
	        ctrs [cptr->suit] += flgs [cptr->suit][cptr->rank];
	        flgs [cptr->suit][cptr->rank] = 0;
	    }
	    cptr = cptr->fwd;
	}
    }

    for (int i = 0; i < 4; i++)
    {
        if (ctrs [i] == 13 && !exposed [i])
	{
	    exposed [i]++;
	    DisplayFullSuitMsg (i);
	}
	else if (ctrs [i] < 13 && exposed [i])
	{
	    exposed [i] = 0;
	    DisplayFullSuitMsg (i);
	}
    }
}


BOOL
IsMoveNatural (int from, int to)
{
    if (!head [from])
	return FALSE;

    if (!head [to])
	return TRUE;

    Card * hi = FindTopOfSuit (from);
    if (tail [from]->suit == tail [to]->suit)
	return (hi->rank == tail [to]->rank - 1);
    else
	return FALSE;
}


BOOL
IsMoveValid (int from, int to)
{
    if (!head [from])	// Can't move FROM an empty column
	return FALSE;

    if (!head [to])	// But we can always move TO an empty column
	return TRUE;

    Card * hi = FindTopOfSuit (from);

    // Cheat - always allow moving a king onto an ace.
    if (hi->rank == 12 && tail[to]->rank == 0)
    	return TRUE;

    if (tail [from]->suit == tail [to]->suit)
	return (tail [from]->rank < tail [to]->rank) &&
	       (hi->rank >= tail [to]->rank - 1);
    else
	return (hi->rank == tail [to]->rank - 1);
}


void
MakeMove (CDC & dc, int from, int to)
{
    Card * hi = FindTopOfSuit (from);

    if (!head [to]) {
        // We're moving to an empty column.

	head [to] = hi;
	tail [to] = tail [from];

	if (!hi->bkw)
	    head [from] = 0;
	else
	    hi->bkw->fwd = 0;

	tail [from] = hi->bkw;
	hi->bkw = 0;
    }
    else {
	// We're moving to an occupied column.

	if (hi->suit == tail [to]->suit) {
	    // Check for the king-on-ace cheat...
	    if (hi->rank < 12 || tail [to]->rank > 0)
		while (hi->rank >= tail [to]->rank)
		    hi = hi->fwd;
	}
	tail [to]->fwd = hi;
	if (!hi->bkw)
	    head [from] = 0;
	else
	    hi->bkw->fwd = 0;

	Card * tmp = tail [to];
	tail [to] = tail [from];
	tail [from] = hi->bkw;
	hi->bkw = tmp;
    }

    // hi points to the highest card we just moved and, for now, still
    // contains its position number in the OLD column.

    ClearColumnTail (dc, from, hi->position);

    RenumberColumn (to);

    // Redraw the next column to the left in case we erased part of it.

    if (pWnd->bClipped && from > 0) {
	DisplayColumn (dc, from - 1);
    }

    // Turn up a card (maybe) and redisplay the new tail.

    if (tail [from]) {
        tail [from]->exposed = 1;
	DisplayCard (dc, tail [from], from);
    }

    // Draw the newly moved cards in their new positions.

    while (hi) {
        DisplayCard (dc, hi, to);
	hi = hi->fwd;
    }

    // Check for full suits.

    CheckSuits ();
}


void
AddCard (int col, Card * cptr)
{
    cptr->fwd = 0;
    cptr->bkw = tail [col];
    cptr->exposed = 1;

    if (!head [col])
    {
	head [col] = cptr;
	cptr->position = 0;
    }
    else
    {
	tail [col]->fwd = cptr;
	cptr->position = tail [col]->position + 1;
    }
    tail [col] = cptr;
}


BOOL
IsRemoveValid (int col)
{
    if (!head [col])
        return FALSE;

    Card * hi = FindTopOfSuit (col);
    return (tail [col]->rank == 0 && hi->rank == 12);
}


void
RemoveSuit (int col)
{
    Card * hi = FindTopOfSuit (col);
    if (hi->bkw)
    {
        hi->bkw->fwd = 0;
	tail [col] = hi->bkw;
	tail [col]->exposed = 1;
    }
    else
    {
	head [col] = tail [col] = 0;
    }
    CheckSuits ();
}


void
DisplayColumn (CDC & dc, int col)
{
    DisplayColumnNumber (dc, col);

    for (Card * cptr = head [col]; cptr; cptr = cptr->fwd)
    {
	DisplayCard (dc, cptr, col);
    }
}

void
DoColumnHints()
{
    if( highlight >= 0 )
    {
	char sz[80];
	char * psz = sz;

	for (Card * cptr = head[highlight]; cptr; cptr = cptr->fwd)
	{
	    if( cptr->exposed )
	    {
		static char szRankMap[] = "A23456789TJQK";
		static char szSuitMap[] = "\xA7\xA8\xA9\xAA";
		
		if( cptr->rank == 10 )
		    *psz++ = '1';
		*psz++ = szRankMap[cptr->rank];
		*psz++ = szSuitMap[cptr->suit];
		*psz++ = ' ';
	    }
	}
	*psz = 0;
	pWnd->SetHint( sz );
    }
    else
	UpdateDeals();
}


int
ColumnLength (int col)
{
    return head [col] ? tail [col]->position + 1 : 0;
}


BOOL
ProcessCommand (CDC & dc, char ch)
{
    pWnd->SetError (" ");

    if (ch >= '0' && ch <= '9') {

	if (automove) {
	    char ok = 0;
	    int score = 0;

    	    automove = FALSE;
	    highlight = ch - '0';	// assume we use this column

	    // Removing is the best plan.

	    if (IsRemoveValid (ch - '0')) {
		ok = 'R';
	    }
	    else for (char col = 0; col < 10; col++) {
		int thisscore = 0;

		if (IsMoveValid (highlight, col)) {
		    if (!head [col]) {
			thisscore = 1;
		    }
		    else if (tail [col]->suit != tail [highlight]->suit) {
			thisscore = 5;
		    }
		    else {
			Card * hifr = FindTopOfSuit (highlight);
			Card * hito = FindTopOfSuit (col);

			if (hifr->rank < hito->rank) {
			    thisscore = 10;
			}
		    }
		}

		if (thisscore > score) {
		    ok = col + '0';
		    score = thisscore;
		}
	    }

	    if (ok > 0) {
		return ProcessCommand (dc, ok);
	    }
	    else {
		DisplayError ("No valid moves.");
		highlight = -1;
		return TRUE;
	    }
	}

//  If a column is highlighted, attempt a move.  If not, highlight it.

	if (highlight >= 0)
	{
	    if (highlight != ch - '0')
	    {
		if (IsMoveValid (highlight, ch - '0'))
		{
		    MakeMove (dc, highlight, ch - '0');
		}
		else
		{
		    DisplayError ("That move won't work.");
		}
	    }

	    // Clear the highlight.

	    DisplayColumnNumber (dc, highlight, FALSE);
	    highlight = -1;
	}
	else
	{
	    highlight = ch - '0';
	    DisplayColumnNumber (dc, highlight, TRUE);
	}
	return TRUE;
    }

    switch (ch) {
	case 'q':
	case 'Q':
	    return FALSE;

	case 'a':
	case 'A':
	    automove = TRUE;
	    if (highlight >= 0) {
		DisplayColumnNumber (dc, highlight, FALSE);
	    }
	    highlight = -1;
	    break;

	case 'd':
	case 'D':		// Deal again
	    if (deals) {
		deals--;
		highlight = -1;
		UpdateDeals ();
		for (int i = 0; i < 10; i++)
		{
		    AddCard (i, next);
		    DisplayCard (dc, next, i);
		    next ++;
		}
		CheckSuits ();
	    }
	    else {
	        return FALSE;
	    }
	    break;

	case 'r':
	case 'R':		// Remove a suit
	    if (highlight < 0)
	    {
	        DisplayError ("No column selected.");
	    }
	    else if (!IsRemoveValid (highlight))
	    {
	        DisplayError ("Not a whole suit.");
	    }
	    else
	    {
	        RemoveSuit (highlight);

		ClearColumn (dc, highlight);
		DisplayColumn (dc, highlight);

		if (pWnd->bClipped && highlight > 0)
		    DisplayColumn (dc, highlight - 1);

		highlight = -1;
		++SuitsRemoved;
		UpdateDeals ();
	        if (SuitsRemoved == 8)
		    return FALSE;
	    }
	    break;

	case '\033':		// Escape
	    if (highlight >= 0) {
		DisplayColumnNumber (dc, highlight, FALSE);
	    }
	    highlight = -1;
	    break;
    }

    return TRUE;
}



void
SpiderInit ()
{
    deals = nDeals;
    SuitsRemoved = 0;
    automove = 0;

    SetupDeck (deck);
    Shuffle (deck);
    next = Deal (deck);

    pWnd->SetSuits ("");
    CheckSuits ();
}
