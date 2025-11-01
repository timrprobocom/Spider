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

Deals = 3
SuitsRemoved = 0

# I need resources like X,Y for the entities.

Point = namedtuple("Point", ['x','y'])
Size = namedtuple("Size", ['x','y'])


BUTTONS = Point(60,20)
BTNSIZE = Size(100,40)
GRID = Point(60,200)

names = 'ace 2 3 4 5 6 7 8 9 10 jack queen king'.split()
suits = 'clubs diamonds hearts spades'.split()
#CARDS = (500,726)

def get_card( cards, suit, idx ):
    pfx = 'PNG-cards-1.3'
    name = f"{pfx}/{names[idx]}_of_{suits[suit]}.png"
    card = Image.open(cards.open(name))
    card = ImageOps.scale( card, 0.25 )
    return pygame.image.fromstring( card.tobytes(), card.size, card.mode ) 

def get_random_card():
    x = random.randrange(0,13)
    y = random.randrange(0,4)
    return cardim[ y*13 + x]

def get_all_cards():
    with zipfile.ZipFile('PNG-cards-1.3.zip') as cards:
        return [get_card(cards, i//13, i%13) for i in range(52)]



COLUMNS = [8, 7, 7, 8, 7, 7, 8, 7, 7, 8]

class Table:
    deck = []
    columns = []
    highlight = None

    def SetupDeck(self):
        deck = []
        with zipfile.ZipFile('PNG-cards-1.3.zip') as cards:
            for i in range(104):
                suit = i // 26
                rank = i % 13
                deck.append( Card( suit, rank, get_card(cards, suit, rank)) )
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
        s = col[-1].suit
        r = col[-1].rank
        n = len(col)-1
        while n > 0:
            n -= 1
            r -= 1
            if col[n].suit != s or col[n].rank != r or not col[n].exposed:
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
        return self.columns[col][-1].rank == 12 and self.columns[col][hi] == 0

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
screen = pygame.display.set_mode( (1400, 800) )
btnfont = pygame.font.SysFont(None, 45)

class Button:
    pos = Point(0,0)
    size = BTNSIZE
    text = None
    hdlr = None

    def __init__(self, txt, pos, hdlr=None):
        self.text = txt
        self.pos = pos
        self.hdlr = hdlr

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

buttons = []
x,y = BUTTONS
for b in ("Deal", "Quit"):
    buttons.append( Button( b, Point(x,y) ) )
    x += BTNSIZE.x + 5

running = True
while running:

    screen.fill( (0,128,0) )
    for b in buttons:
        b.draw()

    for col in range(10):
        for row,card in enumerate(game.columns[col]):
            x = GRID[0] + (cardsize.x+5)*col
            y = GRID[1] + row*40
            screen.blit( card.image, (x, y, cardsize.x, cardsize.y) )

    pygame.display.update()  # Why not flip?

    while True:
        event = pygame.event.wait()
        if event.type == pygame.QUIT:
            running = False
            break
        elif event.type == pygame.KEYUP:
            print(event)
            if event.unicode == 'q':
                running = False
                break
            elif event.unicode == 'd':
                game.Shuffle().Deal()
                break
            
        elif event.type == pygame.MOUSEBUTTONUP:
            print(event.pos, BUTTONS, BTNSIZE)
            for b in buttons:
                if b.contains(Point(*event.pos)):
                    running = False
            break

pygame.quit()
