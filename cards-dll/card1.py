import random
import zipfile
import io
from PIL import Image

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GdkPixbuf, Gio


cards = zipfile.ZipFile('PNG-cards-1.3.zip')
names = 'ace 2 3 4 5 6 7 8 9 10 jack queen king'.split()
suits = 'clubs diamonds hearts spades'.split()
CARDS = (500,726)

def get_card( suit, idx ):
    pfx = 'PNG-cards-1.3'
    name = f"{pfx}/{names[idx]}_of_{suits[suit]}.png"
    buf = GdkPixbuf.Pixbuf.new_from_stream( Gio.MemoryInputStream.new_from_data( cards.read(name) ) )
    neww = buf.get_width()/4
    newh = buf.get_height()/4
    buf = buf.scale_simple(neww, newh, 2)
    return Gtk.Image.new_from_pixbuf(buf)

class Image(Gtk.Window):
    def __init__(self):
        Gtk.Window.__init__(self)
        self.set_title("Image")
        self.connect("destroy", Gtk.main_quit)

        eventbox = Gtk.EventBox()        
        eventbox.connect("button-press-event", self.on_event_press)
        self.add(eventbox)        

        x = random.randrange(0,13)
        y = random.randrange(0,4)
        image = get_card( y, x )
        eventbox.add(image)

    def on_event_press(self, widget, event):
        print('click', widget, event.button, event.time)
        self.close()

window = Image()
window.show_all()

Gtk.main()
