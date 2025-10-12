import os
import sys
import random

class Card:
    suit: int
    rank: int
    exposed: bool
    def __init__( self, suit, rank ):
        self.suit = suit
        self.rank = rank
        self.exposed = False

Deals = 3
SuitsRemoved = 0
highlight = None

BLACK = '\x1b[0;30m'
RED = '\x1b[1;31m'

# HEREIN should be encapsulated all of the platform-specific stuff.

#// For Microsoft C++.

#define clrscr()	_clearscreen (_GCLEARSCREEN)
#define gotoxy(x,y)	_settextposition(y,x)
#define	textbackground(k)	_setbkcolor(k)
#define	textcolor (k)		_settextcolor(k)

card1 = "         1   ";
card2 = "A234567890JQK";
suit  = "SHDC"
suit = [ "\u2660", RED+"\u2665"+BLACK, RED+"\u2666"+BLACK, "\u2663" ]
suit = ["S", RED+"H"+BLACK, RED+"D"+BLACK, "C"]

#define X0	5
#define DX	5
#define Y0	4
#define	DY	1

#define MSG_X	55
#define MSG_Y	5
#define STAT_Y	7
#define ERR_Y	9



## void
## ClearColumn (BYTE col)
## {
##     textbackground (BLUE);
##     window (X0 + col*DX, Y0, X0 + (col+1)*DX - 1, Y0 + 20 * DY);
##     clrscr ();
##     window (1, 1, 80, 25);
## }
## 
## 
## void
## DisplayCard (Card * c, BYTE x, BYTE y)
## {
##     gotoxy (X0 + x * DX, Y0 + y * DY);
##     if (c->exposed)
##     {
## 	textcolor (scol [c->suit]);
## 	textbackground (WHITE);
## 	putch (card1 [c->rank]);
## 	putch (card2 [c->rank]);
## 	putch (suit [c->suit]);
##     }
##     else
##     {
## 	textcolor (BLACK);
## 	textbackground (WHITE);
## 	cputs ("+++");
##     }
## }
## 
## 
## void
## DisplayColumnNumber (BYTE col, BOOL intense = FALSE)
## {
##     gotoxy (X0 + col * DX + 1, Y0 - DY - DY);
##     textcolor (intense ? RED : WHITE);
##     textbackground (BLUE);
##     putch (col + '0');
## 
##     gotoxy (X0 + col * DX, Y0 - DY);
##     textcolor (WHITE);
##     cputs ("===");
## }
## 
## 
## void
## DisplayError (char * s)
## {
##     gotoxy (MSG_X, ERR_Y);
##     textbackground (BLUE);
##     textcolor (WHITE);
##     clreol ();
##     cputs (s);
## }
## 
## 
## void
## UpdateDeals ()
## {
##     char str [80];
## 
##     gotoxy (MSG_X, MSG_Y);
##     textbackground (BLUE);
##     textcolor (WHITE);
##     clreol ();
##     sprintf (str, "%d deals remaining.", deals);
##     cputs (str);
## 
##     if (SuitsRemoved)
##     {
##         gotoxy (MSG_X, STAT_Y);
##         sprintf (str, "%d suits removed.", SuitsRemoved);
## 	cputs (str);
##     }
## }

def ClearColumn(col):
    pass
#    print()
def DisplayColumnNumber(col, highlight=False):
    if highlight:
        print( f'({col}) ', end='')
    else:
        print( f' {col}  ', end='')
def DisplayCard( card, col, y ):
    print( '   '+card1[card.rank]+card2[card.rank]+suit[card.suit], end='' )
def DisplayError( err ):
    if err:
        print(err)
def UpdateDeals():
    print( f"\n {Deals} deals remaining." )
    if SuitsRemoved:
        print( f" {SuitsRemoved} suits removed." )

#// END platform-specific stuff

def SetupDeck():
    deck = []
    for i in range(104):
        deck.append( Card( i // 26, i % 13 ) )
    return deck


def Shuffle( deck ):
    return random.shuffle(deck)

COLUMNS = [8, 7, 7, 8, 7, 7, 8, 7, 7, 8]

def Deal( deck ):
    columns = []
    n = 0
    for c in COLUMNS:
        columns.append( deck[n:n+c] )
        columns[-1][-1].exposed = True
        n += c
    return deck[n:], columns

def FindTopOfSuit( columns, col ):
    s = columns[col][-1].suit
    n = len(columns[col])-1
    while n > 0:
        n -= 1
        if columns[col][n].suit != s or not columns[col][n].exposed:
            return n+1
    return 0

def IsMoveValid( columns, fr, to ):
    if fr is None or not columns[fr]:
        return False
    if not columns[to]:
        return True
    hi = FindTopOfSuit( columns, fr )
    if columns[fr][-1].suit == columns[to][-1].suit:
        return columns[fr][-1].rank < columns[to][-1].rank and \
            (columns[fr][hi].rank >= columns[to][-1].rank - 1)
    return columns[fr][hi].rank >= columns[to][-1].rank - 1


def MakeMove( columns, fr, to ):
    hi = FindTopOfSuit( columns, fr )
    if not columns[to]:
        columns[to] = columns[fr][hi:]
        columns[fr] = columns[fr][:hi]
    else:
        if columns[fr][hi].suit == columns[to][-1].suit:
            while columns[fr][hi].rank >= columns[to][-1].rank:
                hi += 1
        columns[to].extend( columns[fr][hi:] )
        columns[fr] = columns[fr][:hi]
    if columns[fr]:
        columns[fr][-1].exposed = True

def AddCard( columns, col, card ):
    columns[col].append( card )
    card.exposed = True


def IsRemoveValid( columns, col ):
    if not columns[col]:
        return False
    hi = FindTopOfSuit( columns, col )
    return columns[col][-1].rank == 12 and columns[col][hi] == 0



def RemoveSuit( columns, col ):
    hi = FindTopOfSuit(col)
    columns[col] = columns[col][:hi]


def DisplayColumn( columns, col ):
    ClearColumn( col )
    DisplayColumnNumber( col, col==highlight )
    for y, c in enumerate( columns[col] ):
        DisplayCard( c, col, y )
    print()


def DisplayLayout(columns):
    for i in range(10):
        DisplayColumn( columns, i )


def ProcessCommand( deck, columns, ch ):
    global highlight
    global Deals
    global SuitsRemoved
    if ch.isdigit():
        ch = int(ch)
        # If a column is highlighted, attemt a move.  If not, highlight it.
        if highlight is not None:
            if IsMoveValid( columns, highlight, ch ):
                MakeMove( columns, highlight, ch )
                DisplayColumn( columns, highlight )
                DisplayColumn( columns, ch )
                highlight = None
            else:
                DisplayError( "That move won't work." )
                DisplayColumnNumber( highlight )
                highlight = None
        else:
            highlight = ch
            DisplayColumnNumber( highlight, True )

    elif not ch:
        pass

    elif ch in 'Qq':
        return False

    elif ch in 'Dd':    # Deal again
        if Deals:
            Deals -= 1
            highlight = None
            UpdateDeals()
            for i in range(10):
                AddCard( columns, i, deck.pop(0) )
                DisplayColumn(columns, i)

    elif ch in 'Rr':    # Remove a suit
        if highlight is None:
            DisplayError( "No column selected." )
        elif not IsRemoveValid(columns, highlight):
            DisplayError( "Not a whole suit." )
        else:
            RemoveSuit( columns, highlight )
            DisplayColumn( columns, highlight )
            highlight = None
            SuitsRemoved += 1
            if SuitsRemoved == 8:
                return False
            UpdateDeals()

    elif ch == '?': # Refresh
        DisplayLayout(columns)
        UpdateDeals()

    elif ch in 'Xx':    # Cancel highlight -- should be escape
        if highlight is not None:
            DisplayColumnNumber( highlight, False )
            highlight = None

    return True

def main():
    deck = SetupDeck()
    Shuffle(deck)
    deck, columns = Deal(deck)

    DisplayLayout (columns)
    UpdateDeals ()

    while 1:
        ch = input( ">>> " )
        DisplayError("")
        if not ProcessCommand(deck, columns, ch):
            break

    if SuitsRemoved == 8:
        print ("*********************************************")
        print ("*                                           *")
        print ("*        Y O U   W I N   ! ! ! ! ! ! !      *")
        print ("*                                           *")
        print ("*********************************************")

    return 0

if __name__ == "__main__":
    main()
