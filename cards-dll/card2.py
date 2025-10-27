import random
import zipfile
import pygame
from PIL import Image, ImageOps


cards = zipfile.ZipFile('PNG-cards-1.3.zip')
names = 'ace 2 3 4 5 6 7 8 9 10 jack queen king'.split()
suits = 'clubs diamonds hearts spades'.split()
CARDS = (500,726)

# pygame.transform sucks at shrinking.  The docs even say so.
def get_card( suit, idx ):
    pfx = 'PNG-cards-1.3'
    name = f"{pfx}/{names[idx]}_of_{suits[suit]}.png"
    buf = pygame.image.load( cards.open(name) )
    return pygame.transform.scale_by( buf, 1/4 )

def get_card( suit, idx ):
    pfx = 'PNG-cards-1.3'
    name = f"{pfx}/{names[idx]}_of_{suits[suit]}.png"
    card = Image.open(cards.open(name))
    card = ImageOps.scale( card, 0.25 )
    return pygame.image.fromstring( card.tobytes(), card.size, card.mode ) 

def get_random_card():
    x = random.randrange(0,13)
    y = random.randrange(0,4)
    return get_card( y, x )

def get_random_card():
    x = random.randrange(0,13)
    y = random.randrange(0,4)
    return cardim[ y*13 + x]

pygame.init()
screen = pygame.display.set_mode( (1400, 800) )
pygame.display.set_caption("Playing Cards")

# Fetch all of the images.

cardim = []
for i in range(52):
    cardim.append( get_card( i//13, i%13 ) )

# I need a data structure that describes the card and references the image.

iw, ih = cardim[0].get_size()

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    screen.fill( (0,128,0) )

    for col in range(10):
        for row in range(8):
            x = 60 + (iw+5)*col
            y = 20 + row*40
            image = get_random_card()
            screen.blit( image, (x, y, iw, ih) )

    pygame.display.flip()

pygame.quit()
