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

COLUMNS = [8, 7, 7, 8, 7, 7, 8, 7, 7, 8]

class Table:
    deck = []
    columns = []
    highlight = None

    def SetupDeck(self):
        deck = []
        for i in range(104):
            deck.append( Card( i // 26, i % 13 ) )
        self.deck = deck
        return self

    def Shuffle(self):
        random.shuffle(self.deck)
        return self

    def Deal(self):
        columns = []
        n = 0
        for c in COLUMNS:
            columns.append( self.deck[n:n+c] )
            columns[-1][-1].exposed = True
            n += c
        self.deck = self.deck[n:]
        self.columns = columns

    def FindTopOfSuit( self, col ):
        col = self.columns[col]
        s = col[-1].suit
        n = len(col)-1
        while n > 0:
            n -= 1
            if col[n].suit != s or not col[n].exposed:
                return n+1
        return 0

    def IsMoveValid( self, fr, to ):
        if fr is None or not self.columns[fr]:
            return False
        if not self.columns[to]:
            return True
        hi = self.FindTopOfSuit( fr )
        if self.columns[fr][-1].suit == self.columns[to][-1].suit:
            return self.columns[fr][-1].rank < self.columns[to][-1].rank and \
                  (self.columns[fr][hi].rank >= self.columns[to][-1].rank - 1)
        return self.columns[fr][hi].rank >= self.columns[to][-1].rank - 1

    def MakeMove( self, fr, to ):
        hi = self.FindTopOfSuit( fr )
        if not self.columns[to]:
            self.columns[to] = self.columns[fr][hi:]
            self.columns[fr] = self.columns[fr][:hi]
        else:
            if self.columns[fr][hi].suit == self.columns[to][-1].suit:
                while self.columns[fr][hi].rank >= self.columns[to][-1].rank:
                    hi += 1
            self.columns[to].extend( self.columns[fr][hi:] )
            self.columns[fr] = self.columns[fr][:hi]
        if self.columns[fr]:
            self.columns[fr][-1].exposed = True

    def AddCard( self, col, card ):
        self.columns[col].append( card )
        card.exposed = True

    def IsRemoveValid( self, col ):
        if not self.columns[col]:
            return False
        hi = self.FindTopOfSuit( col )
        return self.columns[col][-1].rank == 12 and self.columns[col][hi] == 0

    def RemoveSuit( self, col ):
        hi = self.FindTopOfSuit(col)
        self.columns[col] = self.columns[col][:hi]

    def DisplayColumn( self, col ):
        ClearColumn( col )
        DisplayColumnNumber( col, col==self.highlight )
        for y, c in enumerate( self.columns[col] ):
            DisplayCard( c, col, y )
        print()

    def DisplayLayout(self):
        for i in range(10):
            self.DisplayColumn( i )

    def IsHighlight(self):
        return self.highlight is not None

    def SetHighlight(self,value=None):
        self.highlight = value

    def ClearHighlight(self):
        self.highlight = None


def ProcessCommand( table, ch ):
    global Deals
    global SuitsRemoved
    if ch.isdigit():
        ch = int(ch)
        # If a column is highlighted, attemt a move.  If not, highlight it.
        if table.IsHighlight():
            if table.IsMoveValid( table.highlight, ch ):
                table.MakeMove( table.highlight, ch )
                table.DisplayColumn( table.highlight )
                table.DisplayColumn( ch )
                table.ClearHighlight()
            else:
                DisplayError( "That move won't work." )
                DisplayColumnNumber( highlight )
                table.ClearHighlight()
        else:
            table.SetHighlight(ch)
            DisplayColumnNumber( table.highlight, True )

    elif not ch:
        pass

    elif ch in 'Qq':
        return False

    elif ch in 'Dd':    # Deal again
        if Deals:
            Deals -= 1
            table.ClearHighlight()
            UpdateDeals()
            for i in range(10):
                table.AddCard( i, deck.pop(0) )
                table.DisplayColumn(i)

    elif ch in 'Rr':    # Remove a suit
        if highlight is None:
            DisplayError( "No column selected." )
        elif not IsRemoveValid(columns, highlight):
            DisplayError( "Not a whole suit." )
        else:
            table.RemoveSuit( highlight )
            table.DisplayColumn( highlight )
            table.ClearHighlight()
            SuitsRemoved += 1
            if SuitsRemoved == 8:
                return False
            UpdateDeals()

    elif ch == '?': # Refresh
        table.DisplayLayout()
        UpdateDeals()

    elif ch in 'Xx':    # Cancel highlight -- should be escape
        if table.IsHighlight():
            DisplayColumnNumber( table.highlight, False )
            table.ClearHighlight()

    return True

def main():
    game = Table()
    game.SetupDeck().Shuffle().Deal()

    game.DisplayLayout()
    UpdateDeals ()

    while 1:
        ch = input( ">>> " )
        DisplayError("")
        if not ProcessCommand(game, ch):
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
