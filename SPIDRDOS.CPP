#include <conio.h>
#include <graph.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned short BOOL;
typedef unsigned long DWORD;

#define FALSE	0
#define TRUE	-1


class Card
{
public:			// I have no shame.
    BYTE suit;
    BYTE rank;
    BOOL exposed;
    Card * fwd;
    Card * bkw;
};


Card deck [104];
Card * next;

Card * head [10];
Card * tail [10];

int deals = 3;
int SuitsRemoved = 0;

BYTE highlight = 0xff;


// HEREIN should be encapsulated all of the platform-specific stuff.

// For Microsoft C++.

#define clrscr()	_clearscreen (_GCLEARSCREEN)
#define gotoxy(x,y)	_settextposition(y,x)
#define	textbackground(k)	_setbkcolor(k)
#define	textcolor (k)		_settextcolor(k)

char card1 [] = "         1   ";
char card2 [] = "A234567890JQK";
char suit [] = "\006\003\004\005";		// Spades, Hearts, Diamonds, Clubs
int scol [] = {BLACK, RED, RED, BLACK};

#define X0	5
#define DX	5
#define Y0	4
#define	DY	1

#define MSG_X	55
#define MSG_Y	5
#define STAT_Y	7
#define ERR_Y	9



void
ClearColumn (BYTE col)
{
    textbackground (BLUE);
    window (X0 + col*DX, Y0, X0 + (col+1)*DX - 1, Y0 + 20 * DY);
    clrscr ();
    window (1, 1, 80, 25);
}


void
DisplayCard (Card * c, BYTE x, BYTE y)
{
    gotoxy (X0 + x * DX, Y0 + y * DY);
    if (c->exposed)
    {
	textcolor (scol [c->suit]);
	textbackground (WHITE);
	putch (card1 [c->rank]);
	putch (card2 [c->rank]);
	putch (suit [c->suit]);
    }
    else
    {
	textcolor (BLACK);
	textbackground (WHITE);
	cputs ("+++");
    }
}


void
DisplayColumnNumber (BYTE col, BOOL intense = FALSE)
{
    gotoxy (X0 + col * DX + 1, Y0 - DY - DY);
    textcolor (intense ? RED : WHITE);
    textbackground (BLUE);
    putch (col + '0');

    gotoxy (X0 + col * DX, Y0 - DY);
    textcolor (WHITE);
    cputs ("===");
}


void
DisplayError (char * s)
{
    gotoxy (MSG_X, ERR_Y);
    textbackground (BLUE);
    textcolor (WHITE);
    clreol ();
    cputs (s);
}


void
UpdateDeals ()
{
    char str [80];

    gotoxy (MSG_X, MSG_Y);
    textbackground (BLUE);
    textcolor (WHITE);
    clreol ();
    sprintf (str, "%d deals remaining.", deals);
    cputs (str);

    if (SuitsRemoved)
    {
        gotoxy (MSG_X, STAT_Y);
        sprintf (str, "%d suits removed.", SuitsRemoved);
	cputs (str);
    }
}


// END platform-specific stuff

void
SetupDeck (Card * d)
{
    int i;

    for (i = 0; i < 104; i++, d++)
    {
	d->suit = i / 26;
	d->rank = i % 13;
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
	a = (((DWORD) rand ()) * 104) >> 15;
	b = (((DWORD) rand ()) * 104) >> 15;
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
    static short size [] = { 8, 7, 7, 8, 7, 7, 8, 7, 7, 8 };

    for (i = 0; i < 10; i++)
    {
	head [i] = cptr;
	cptr->bkw = 0;
	cptr += size [i] - 1;
	cptr->fwd = 0;
	cptr -> exposed = 1;
	tail [i] = cptr;
	cptr ++;
    }

    return cptr;
}


Card *
FindTopOfSuit (BYTE col)
{
    BYTE s = tail [col]->suit;
    Card * cptr = tail [col];

    while (cptr->bkw && s == cptr->suit && cptr->exposed)
	cptr = cptr->bkw;

    if (!cptr->bkw && s == cptr->suit && cptr->exposed)
	return cptr;
    else
	return cptr->fwd;
}


BOOL
IsMoveValid (BYTE from, BYTE to)
{
    if (!head [from])
	return FALSE;

    if (!head [to])
	return TRUE;

    Card * hi = FindTopOfSuit (from);
    if (tail [from]->suit == tail [to]->suit)
	return (tail [from]->rank < tail [to]->rank) &&
	       (hi->rank >= tail [to]->rank - 1);
    else
	return (hi->rank == tail [to]->rank - 1);
}


void
MakeMove (BYTE from, BYTE to)
{
    Card * hi = FindTopOfSuit (from);

    if (!head [to])
    {
	head [to] = hi;
	tail [to] = tail [from];
	if (!hi->bkw)
	    head [from] = 0;
	else
	    hi->bkw->fwd = 0;

	tail [from] = hi->bkw;
	hi->bkw = 0;
    }
    else
    {
	if (hi->suit == tail [to]->suit)
	{
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

    if (tail [from])
	tail [from]->exposed = 1;
}


void
AddCard (BYTE col, Card * cptr)
{
    cptr->exposed = 1;
    cptr->fwd = 0;
    cptr->bkw = tail [col];

    if (!head [col])
    {
	head [col] = cptr;
    }
    else
    {
	tail [col]->fwd = cptr;
    }
    tail [col] = cptr;
}


BOOL
IsRemoveValid (BYTE col)
{
    if (!head [col])
        return FALSE;

    Card * hi = FindTopOfSuit (col);
    return (tail [col]->rank == 12 && hi->rank == 0);
}


void
RemoveSuit (BYTE col)
{
    Card * hi = FindTopOfSuit (col);
    if (hi->bkw)
    {
        hi->bkw->fwd = 0;
	tail [col] = hi->bkw;
    }
    else
    {
	head [col] = tail [col] = 0;
    }
}


void
DisplayColumn (BYTE col)
{
    BYTE y;

    ClearColumn (col);
    DisplayColumnNumber (col);
    y = 0;
    for (Card * cptr = head [col]; cptr; cptr = cptr->fwd)
    {
	DisplayCard (cptr, col, y);
	y ++;
    }
}


void
DisplayLayout ()
{
    int i;

    for (i = 0; i < 10; i++)
	DisplayColumn (i);
}


BOOL
ProcessCommand (char ch)
{
    if (ch >= '0' && ch <= '9')
    {

//  If a column is highlighted, attempt a move.  If not, highlight it.

	if (highlight < 10)
	{
	    if (IsMoveValid (highlight, ch - '0'))
	    {
		MakeMove (highlight, ch - '0');
		DisplayColumn (highlight);
		DisplayColumn (ch - '0');
		highlight = 0xff;
	    }
	    else
	    {
		DisplayError ("That move won't work.");
		DisplayColumnNumber (highlight);
		highlight = 0xff;
	    }
	}
	else
	{
	    highlight = ch - '0';
	    DisplayColumnNumber (highlight, TRUE);
	}
	return TRUE;
    }

    switch (ch)
    {
	case 'q':
	case 'Q':
	    return FALSE;

	case 'd':
	case 'D':		// Deal again
	    if (deals)
	    {
		deals--;
		highlight = 0xff;
		UpdateDeals ();
		for (int i = 0; i < 10; i++)
		{
		    AddCard (i, next);
		    next ++;
		    DisplayColumn (i);
		}
	    }
	    break;

	case 'r':
	case 'R':		// Remove a suit
	    if (highlight >= 10)
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
		DisplayColumn (highlight);
		highlight = 0xff;
	        if (++SuitsRemoved == 8)
		    return FALSE;
		UpdateDeals ();
	    }
	    break;

	case '\033':		// Escape
	    if (highlight < 0xff)
		DisplayColumnNumber (highlight, FALSE);
	    highlight = 0xff;
	    break;
    }

    return TRUE;
}


int
main ()
{
    char ch;

    _setvideomoderows (_TEXTCO80, 25);

    randomize ();
    clrscr ();

    SetupDeck (deck);
    Shuffle (deck);
    next = Deal (deck);

    DisplayLayout ();
    UpdateDeals ();

    do
    {
	ch = getch ();
	DisplayError ("");
    }
    while (ProcessCommand (ch));

    if (SuitsRemoved == 8)
    {
        clrscr ();
	printf ("*********************************************\n");
	printf ("*                                           *\n");
	printf ("*        Y O U   W I N   ! ! ! ! ! ! !      *\n");
	printf ("*                                           *\n");
	printf ("*********************************************\n");
    }

    return 0;
}
