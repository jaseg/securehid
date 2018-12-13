#!/usr/bin/env python3

import itertools

import gi
import gi
from gi.repository import Gtk as gtk, Gdk as gdk, Pango as pango, GLib as glib

import numpy as np


class PyApp(gtk.Window):
    def __init__(self, m=5):
        super(PyApp, self).__init__()

        self.set_title("Mouse pairing mockup")
        self.set_default_size(400, 400)
        vbox = gtk.VBox()
        grid = gtk.Grid(row_spacing=20, column_spacing=20, row_homogeneous=True, column_homogeneous=True, margin=20)
        lbl = self.lbl = gtk.Label('Please press the buttons below in order 1-2-3...')

        # GTK has the best APIs... -.-
        screen = gdk.Screen.get_default()
        gtk_provider = gtk.CssProvider()
        gtk_context = gtk.StyleContext()
        gtk_context.add_provider_for_screen(screen, gtk_provider, gtk.STYLE_PROVIDER_PRIORITY_APPLICATION)
        css = b'''
            #pinpad > button {font: 24 bold}
            #pinpad > button:checked {background: linear-gradient(#e5efd4, #588a5a)}
        '''
        grid.set_name('pinpad')
        gtk_provider.load_from_data(css)

        st = np.random.RandomState(seed=0)
        for i, (x, y) in zip(st.permutation(m**2), itertools.product(range(m), range(m))):
            btn = gtk.ToggleButton(f'{i+1}')
            btn.connect("toggled", self.on_toggled)
            grid.attach(btn, x, y, 1, 1)

        vbox.pack_start(lbl, True, True, 0)
        vbox.pack_start(grid, True, True, 0)
        self.add(vbox)
        self.connect("destroy", gtk.main_quit)
        self.show_all()

    def on_toggled(self, widget, data=None):
        print(f'Clicked : {widget.get_label()}')

if __name__ == '__main__':
    PyApp()
    gtk.main()
