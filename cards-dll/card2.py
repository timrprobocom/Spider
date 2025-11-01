import random
import zipfile
import pygame
from collections import namedtuple
from PIL import Image, ImageOps

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

cardim = get_all_cards()

# I need a data structure that describes the card with a reference to the image.

cardsize = Size(*cardim[0].get_size())

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
        for row in range(8):
            x = GRID[0] + (cardsize.x+5)*col
            y = GRID[1] + row*40
            image = get_random_card()
            screen.blit( image, (x, y, cardsize.x, cardsize.y) )

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
            
        elif event.type == pygame.MOUSEBUTTONUP:
            print(event.pos, BUTTONS, BTNSIZE)
            for b in buttons:
                if b.contains(Point(*event.pos)):
                    running = False
            break

pygame.quit()
