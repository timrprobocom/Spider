import random
import zipfile
import io
from PIL import Image


cards = zipfile.ZipFile('PNG-cards-1.3.zip')
names = 'ace 2 3 4 5 6 7 8 9 10 jack queen king'.split()
suits = 'clubs diamonds hearts spades'.split()
def get_card( suit, idx ):
    pfx = 'PNG-cards-1.3'
    name = f"{pfx}/{names[idx]}_of_{suits[suit]}.png"
    return Image.open(cards.open(name))

x = random.randrange(0,13)
y = random.randrange(0,4)
get_card( y, x ).show()
