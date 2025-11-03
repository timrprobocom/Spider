#! /usr/bin/env python3

import os
import random
import zipfile
import pygame
from collections import namedtuple
from PIL import Image, ImageOps


class Card:
    suit: int
    rank: int
    exposed: bool
    image: None
    def __init__( self, suit, rank, image ):
        self.suit = suit
        self.rank = rank
        self.image = image
        self.exposed = False
    def __repr__( self ):
        return "A23456789TJQK"[self.rank]+"CDHS"[self.suit]


COLUMNS = [8, 7, 7, 8, 7, 7, 8, 7, 7, 8]
Deals = 3
COLUMNS = [5, 4, 4, 5, 4, 4, 5, 4, 4, 5]
Deals = 6
SuitsRemoved = 0

# I need resources like X,Y for the entities.

Point = namedtuple("Point", ['x','y'])
Size = namedtuple("Size", ['x','y'])


BUTTONS = Point(60,20)
BTNSIZE = Size(120,40)
GRID = Point(60,200)

STAT = Point(60,100)
MSG = Point(640,100)


CARDZIP = 'PNG-cards-1.3.zip'
names = 'ace 2 3 4 5 6 7 8 9 10 jack queen king'.split()
suits = 'clubs diamonds hearts spades'.split()
factor = 0.25

CARDZIP = 'cards_png_zip.zip'
names = 'A 2 3 4 5 6 7 8 9 10 J Q K'.split()
suits = 'CDHS'
factor = 0.2

class CardZip:
    def __init__(self):
        we = os.path.dirname(__file__)
        self.cards = zipfile.ZipFile(we+"/"+CARDZIP)

    def get_card_image( self, name ):
        card = Image.open(self.cards.open(name))
        card = ImageOps.scale( card, factor )
        return pygame.image.fromstring( card.tobytes(), card.size, card.mode ) 

    def get_card( self, suit, idx ):
        pfx = 'PNG'
#    pfx = 'PNG-cards-1.3'
        name = f"{pfx}/{names[idx]}{suits[suit]}.png"
#    name = f"{pfx}/{names[idx]}_of_{suits[suit]}.png"
        return self.get_card_image( name )

    def get_all_cards(self):
        return [self.get_card(i//13, i%13) for i in range(52)]


class Table:
    deck = []
    columns = []
    highlight = None

    def SetupDeck(self):
        deck = []
        cards = CardZip()
        for i in range(104):
            suit = i // 26
            rank = i % 13
            deck.append( Card( suit, rank, cards.get_card(suit, rank)) )
        self.back = cards.get_card_image( "PNG/blue_back.png" )
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
        self.next = n
        self.columns = columns

    def FindTopOfSuit( self, col ):
        col = self.columns[col]
        n = len(col)-1
        s = col[n].suit
        r = col[n].rank
        while n > 0:
            n -= 1
            r += 1
            if col[n].suit != s or col[n].rank != r or not col[n].exposed:
                return n+1
        return 0

    def IsMoveValid( self, fr, to ):
        if fr is None or not self.columns[fr]:
            return False
        hi = self.FindTopOfSuit( fr )
        if not self.columns[to]:
            return hi
#        print(fr,to,hi,self.columns[fr][hi],self.columns[to][-1])
        fr = self.columns[fr]
        to = self.columns[to]

        if fr[hi].rank == to[-1].rank - 1:
            return hi

        # REMEMBER you can move part of a long suit.  So, given 6 5 4 3 spades,
        # I can move the 4 to a 5 spades.

        if fr[hi].suit == to[-1].suit and fr[hi].rank >= to[-1].rank > fr[-1].rank:
            return len(fr) - (to[-1].rank - fr[-1].rank)

        #  This is a cheat: you can move a column headed by a king to any ace.

        if fr[hi].rank == 12 and to[-1].rank == 0:
            return hi
        return -1


    def MakeMove( self, fr, to ):
        hi = self.IsMoveValid( fr, to )
        if hi < 0:
            return
        if not self.columns[to]:
            self.columns[to] = self.columns[fr][hi:]
            self.columns[fr] = self.columns[fr][:hi]
        else:
            self.columns[to].extend( self.columns[fr][hi:] )
            self.columns[fr] = self.columns[fr][:hi]
        if self.columns[fr]:
            self.columns[fr][-1].exposed = True

    def DealOneCard( self, col ):
        self.AddCard( col, self.deck[self.next] )
        self.next += 1

    def AddCard( self, col, card ):
        self.columns[col].append( card )
        card.exposed = True

    def IsRemoveValid( self, col ):
        if not self.columns[col]:
            return False
        hi = self.FindTopOfSuit( col )
        col = self.columns[col]
        return col[-1].rank == 0 and col[hi].rank == 12

    def RemoveSuit( self, col ):
        hi = self.FindTopOfSuit(col)
        self.columns[col] = self.columns[col][:hi]

    def DisplayColumn( self, col ):
        ClearColumn( col )
        DisplayColumnNumber( col, col==self.highlight )
        for y, c in enumerate( self.columns[col] ):
            DisplayCard( c, col, y )

    def DisplayLayout(self):
        for i in range(10):
            self.DisplayColumn( i )

    def IsHighlight(self):
        return self.highlight is not None

    def SetHighlight(self,value=None):
        self.highlight = value

    def ClearHighlight(self):
        self.highlight = None




pygame.init()
pygame.display.set_caption("Playing Cards")
screen = pygame.display.set_mode( (1600, 1000), pygame.RESIZABLE )
btnfont = pygame.font.SysFont(None, 45)

class Button:
    pos = Point(0,0)
    size = BTNSIZE
    text = None
    hdlr = None
    key = ''

    def __init__(self, txt, pos, hdlr=None):
        self.text = txt
        self.pos = pos
        self.hdlr = hdlr
        self.key = txt[0].lower()

    def draw(self):
        drawButton( self.text, self.pos )
    
    def contains(self, pt):
        return self.pos.x <= pt.x <= self.pos.x+self.size.x and \
            self.pos.y <= pt.y <= self.pos.y+self.size.y

    def click(self):
        if hdlr:
            hdlr()

def drawButton(txt,loc):
    pygame.draw.rect( screen, (0,0,0), loc+BTNSIZE, width=2, border_radius=14)
    size = Size(*btnfont.size(txt))
    bx,by = BTNSIZE
    x = loc.x + (bx - size.x) / 2
    y = loc.y + (by - size.y) / 2
    txt = btnfont.render( txt, True, (255,255,255) )
    screen.blit( txt, (x, y) )

# Fetch all of the images.

game = Table()
game.SetupDeck().Shuffle().Deal()

cardsize = Size(*game.deck[0].image.get_size())
xspacing = 5
yspacing = 60

buttons = []
x,y = BUTTONS
for b in ("Deal", "Remove", "Clear", "Quit"):
    buttons.append( Button( b, Point(x,y) ) )
    x += BTNSIZE.x + 5

error = 'Version 1.0'
running = True
while running:

    # Clear the table.

    screen.fill( (0,128,0) )

    # Draw the buttons.

    for b in buttons:
        b.draw()

    # Draw the tableau.

    w = cardsize.x
    h = cardsize.y
    for col in range(10):
        x = GRID[0] + (cardsize.x+xspacing)*col
        txt = btnfont.render( str(col), True, (255,255,255) )
        screen.blit( txt, (x+w/2-25, GRID[1]-45) )
        for row,card in enumerate(game.columns[col]):
            y = GRID[1] + row*yspacing
            if card.exposed:
                screen.blit( card.image, (x, y, w, h) )
            else:
                screen.blit( game.back, (x, y, w, h) )
            pygame.draw.rect( screen, (0,0,0), (x,y,w,h), width=2, border_radius=10)
        if col == game.highlight:
            xx = x - 3
            ww = w + 6
            yy = GRID[1] - 3
            hh = y + h + 3 - yy
            pygame.draw.rect( screen, (255,255,255), (xx,yy,ww,hh), width=3 )

    msg = f"{Deals} deals remaining."
    if SuitsRemoved:
        msg += f"   {SuitsRemoved} suits removed."
    txt = btnfont.render( msg, True, (255,255,255) )
    screen.blit( txt, STAT )

    # Draw any err message.

    if error:
        txt = btnfont.render( error, True, (255,255,255) )
        screen.blit( txt, MSG )
        error = ''

    # Go display.

    pygame.display.update()

    # Process input.

    while True:
        event = pygame.event.wait()
        action = ''
        if event.type == pygame.QUIT:
            action = 'q'
        elif event.type == pygame.VIDEORESIZE:
            break
        elif event.type == pygame.KEYUP:
            action = event.unicode
        elif event.type == pygame.MOUSEBUTTONUP:
            for b in buttons:
                if b.contains(Point(*event.pos)):
                    action = b.key

        if action.isdigit():
            ch = int(action)
            # If a column is highlighted, attempt a move.  If not, highlight it.
            if game.IsHighlight():
                h = game.highlight
                if game.IsMoveValid( game.highlight, ch ) >= 0:
                    game.MakeMove( game.highlight, ch )
                    game.ClearHighlight()
                else:
                    error = "That move won't work."
                    game.ClearHighlight()
            else:
                game.SetHighlight(ch)
            break
        elif action == 'q':
            running = False
            break
        elif action == 'd':
            if Deals:
                Deals -= 1
                game.ClearHighlight()
                for i in range(10):
                    game.DealOneCard( i )
            break
        elif action == 'r':
            if not game.IsHighlight():
                error = "No column highlighted"
                break
            if not game.IsRemoveValid(game.highlight):
                error = "Cannot remove that column"
                break
            game.RemoveSuit(game.highlight)
            game.ClearHighlight()
            SuitsRemoved += 1
            if SuitsRemoved == 8:
                error = "YOU WIN!!!"
            break

        elif action == 'c':
            game.Shuffle().Deal()
            break

pygame.quit()
