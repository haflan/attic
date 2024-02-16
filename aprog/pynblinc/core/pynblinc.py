#!/bin/python
"""
The starting point of pynblinc
Loads setup from pynsetup (which in turn loads pyncore)
Initializes curses and csound and starts the run loop
"""

# [M] - Lines with this tag are temporarily commented out for phone programming

import Pattern, Events
import pyncore
import mitext
import sys

import ctcsound
from subprocess import run
import curses
import operator

# Modal editor, yo
from enum import Enum
class Mode(Enum):
    PLAY = 0
    EDIT = 1
    ACTION = 2

# TODO: Let csoundfile be a sysarg
orcfile = "porta.orc"
scofile = "porta.sco"
orc = ""
sco = ""

# use this while testing so that file input is optional
if len(sys.argv) == 2:
    project_name = sys.argv[1]
    orcfile = project_name + ".orc"
    scofile = project_name + ".sco"

# Read the orc file (which should exist!)
try:
    with open(orcfile, 'r') as f:
        orc = f.read()
        f.close()
except FileNotFoundError:
    print("{} not found. You need to make an orchestra file before composing a score".format(orcfile))
    exit(1)

# Read the sco file or create it if it doesn't exist.
with open(scofile, 'r+') as f:
    sco = [ line.strip() for line in f.readlines() ]
    f.close()


##### TODO: Clean up this. Put it in pyncore or something?
# This is sort of a necessary hack to be able to use
csd = """
<CsoundSynthesizer>
<CsOptions>
-d -o dac -m0
</CsOptions>
<CsInstruments>
{}
</CsInstruments>
<CsScore>
f 0  14400
</CsScore>
</CsoundSynthesizer>
""".format(orc)
# Maybe it is not necessary after all. Rewrite if you ever get the time. Look:
# https://github.com/csound/csoundAPI_examples/blob/master/python/example11.py
# Notice that this uses csnd6 and not ctcsound, though!
##### /end cleanup

tracks = []

### SETUP ###
def init_csound():
# Creates a new instance of Csound AND a new performance thread
    cs = ctcsound.Csound()
    cs.compileCsdText(csd)
    cs.start()
    return cs

def stop_csound(pt):
    pt.stop()
    pt.join()

def init_cpad(max_y, max_x):
    pad = curses.newpad(2000, max_x)
    return pad

def draw_events(stdscr, pad, pattern):
    # TODO: Sort events before drawing to pad
    # pattern.events_list.sort(key=operator.attrgetter('start'))
    #curses.move(ev_num,0)
    (max_y, max_x) = stdscr.getmaxyx()
    for ev_num, event in enumerate(pattern.events_list):
        if ev_num == pattern.active_event:
            pad.addstr(ev_num, 0, str(event), curses.A_STANDOUT)
        else:
            pad.addstr(ev_num, 0, str(event))
    #pad.border(0)  # add borders / box later
    display_from_event = int((pattern.active_event)/(max_y-3))*(max_y-3)
    pad.refresh(display_from_event,0,1,4,max_y-3,int(max_x/2))

def main(stdscr):

    # TODO: Consider moving the Csound init outside of the curses wrapper...
    # init csound and a performance thread
    cs = init_csound()
    pt = ctcsound.CsoundPerformanceThread(cs.csound())

    # Load pattern from sco file and assign it the performance thread pt
    #pattern = mitext.midi_to_pattern('/home/vh/sonic/melodies/Funky Feelin\'.mid', pt)
    # NOTE: Pattern is both the module and the class name. This is kinda stupid
    pattern = Pattern.Pattern.from_sco(pt, sco)
    # ^ This must be set before the pthread is started.
    #pattern = Pattern.Pattern(pt)
    #print(pattern)

    pt.play()

    # init curses + windows
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    (max_y, max_x) = stdscr.getmaxyx()
    pattern_pad = init_cpad(max_y, max_x)
    curses.curs_set(0)
    #stdscr.refresh()
    #win.refresh()
    # [curses pads](https://gist.github.com/richarddun/5f732525e2e909479970d99c17b6dbbb)


#   # TODO: Reconsider whether a modal editor is needed.
    mode = Mode.EDIT
    running = True
    current_note = "8.00"
    play_event = Events.Event([1, 3.4, 0.5, 8.03])

    stdscr.addstr(0, 4, pattern.name, curses.A_UNDERLINE)
    redraw = True
    BACKSPACE = curses.erasechar()
    while running:
        while mode == Mode.PLAY:
            # Using the keyboard as a piano. No need to redraw any windows
            c = stdscr.getch()
            if c in play_keymaps:
                play_event.note = "7" + play_keymaps[c]
                play_event.play(pt)
                #stdscr.addstr(0,0, play_event.note + "\n")
        while mode == Mode.EDIT:
            # Redraw only if needed
            if redraw:
                draw_events(stdscr, pattern_pad, pattern)
            c = stdscr.getch()
            if c in pyncore.edit_keymaps:
                # c will be the key to the function to call
                redraw = pyncore.edit_keymaps[c](pattern)
            elif c == ord(':'):
                mode = Mode.ACTION
                curses.curs_set(1)
                stdscr.move(max_y - 1, 0)
                stdscr.addch(':')
        while mode == Mode.ACTION:
            #string = curses.getstr()
            c = stdscr.getch()
            stdscr.addch(c)
            # TODO: Find out why this does not work
            if c == BACKSPACE:
                mode = Mode.EDIT
                break
            elif c == ord('q'):
                running = False
                break
            elif c == ord('w'):
                pattern.to_sco(scofile)
                mode = Mode.EDIT

#while mode == Mode.EDIT:
    # TODO: Set tempo parameter
    #p = ctcsound.CsoundParams()
    #cs.params(p)
    #print(p._fields_)
    stop_csound(pt)
    curses.nocbreak()
    curses.endwin()

curses.wrapper(main)
