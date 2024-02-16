"""
pynblinc classes and main functionality
"""
#import ctcsound
import re
import operator     # for sorting events lists
import Events
import Pattern

### Resources
# - [1 CSound API](https://csound.com/docs/api/index.html)
# - [2 ctcsound](https://github.com/fggp/ctcsound/blob/master/ctcsound.py)
# ctcsound is a wrapper for libcsound, if I am not mistaken. Link 1 shows some
# examples on Csound functions / methods you can use for playing scores etc.
# Link 2 shows you how these methods are implemented in Python.

### SETUP ###

# TODO: Read the keyboard mappings from the config file
### Parse config file
configs = {}
try:
    with open("pynblinc.conf") as f:
        conflines = f.readlines()
        for ln, line in enumerate(conflines):
            line = line.strip(' ').replace(' ', '') # remove \n and spaces
            match = re.search("(.+)=(.+)", line)
            if match:
                configs[match.group(1)] = match.group(2)
            if '=' in line and not match:
                print("Syntax error in line {} of config".format(ln+1))
except Exception as e:
    print("Error opening config file: {}".format(e))
    exit(1)

### Functions
### CORE FUNCTIONALITY OF PYNBLINC ###
# TODO: Event manipulation functionality should belong to Pattern class.
def event_add(pattern):
    #print("added event")
    # TODO: Rewrite this to use a nev *object*
    nev = new_event_value
    new_event = Events.Event([nev["inst"], nev["start"], nev["dur"], 60])
    pattern.add_event(new_event)
    return True
def event_insert(pattern):
    #nev = new_event_value
    #new_event = Events.Event([nev["inst"], nev["start"], nev["dur"], 60])
    # Actually, copy the active event
    ae = pattern.active_event
    new_event = Events.Event(pattern.events_list[ae].pfields[:])
    pattern.insert_event(new_event)
    return True

def event_down(pattern):
    if pattern.active_event < pattern.num_events() - 1:
        pattern.active_event += 1
    return True
def event_up(pattern):
    if pattern.active_event > 0:
        pattern.active_event -= 1
    return True

def octave_up(pattern):
    return True


## Editor settings (can be overwritten during runtime)
# Default Step sizes; number of beats to increase/decrease a value by when using
# keyboard to scroll through values.
step_size = {
        "inst":  int(configs["STEP_INST"]),     # instrument. Is this useful at all, though?
        "dur":   float(configs["STEP_DUR"]),    # event duration
        "start": float(configs["STEP_START"])   # event start. also called timestamp some places. clean up that stuff
        }

def event_instup(pattern):
    ae = pattern.active_event
    should_redraw = pattern.events_list[ae].increase_pfield(0, 1, 16)
    return should_redraw

def event_instdown(pattern):
    ae = pattern.active_event
    should_redraw = pattern.events_list[ae].increase_pfield(0, -1, 1)
    return should_redraw

def event_ts_dec(pattern):
    ae = pattern.active_event
    should_redraw = pattern.events_list[ae].increase_pfield(1, -step_size["start"], 0.0)
    # The limit for float fields must be given as float,                            /\
    # or the field will be converted to an int by the set_pfield method of the event.
    pattern.fix_order(-1)
    return should_redraw

def event_ts_inc(pattern):
    ae = pattern.active_event
    should_redraw = pattern.events_list[ae].increase_pfield(1, step_size["start"], None)
    pattern.fix_order(1)
    return should_redraw

def event_dur_dec(pattern):
    ae = pattern.active_event
    should_redraw = pattern.events_list[ae].increase_pfield(2, -step_size["dur"], -1.0)
    return should_redraw

def event_dur_inc(pattern):
    ae = pattern.active_event
    should_redraw = pattern.events_list[ae].increase_pfield(2, step_size["dur"], None)
    return True

def event_note_inc(pattern):
    ae = pattern.active_event
    should_redraw = pattern.events_list[ae].increase_pfield(3, 1, 128)
    return True

def event_note_dec(pattern):
    ae = pattern.active_event
    should_redraw = pattern.events_list[ae].increase_pfield(3, -1, 50)
    return True

def event_play(pattern):
    pattern.play_event()
    return False

def pattern_play(pattern):
    pattern.toggleplay()
    return False

def tsstep_1(pattern):
    step_size["start"] = 0.100
    step_size["dur"] = 0.100

def tsstep_2(pattern):
    step_size["start"] = 0.050
    step_size["dur"] = 0.050

def tsstep_3(pattern):
    step_size["start"] = 1.0/30.0
    step_size["dur"] = 1.0/30.0


# TODO: Generalizing like this (function(pattern)) is just stupid when the functions are so different
edit_keymaps = {
        ord(configs["KEY_UP"]):       event_up,  # add current_note (last played)
        ord(configs["KEY_DOWN"]):     event_down, # go to next event in list
        ord(configs["KEY_ADD"]):      event_add,   # go to previous event
        ord(configs["KEY_INSERT"]):   event_insert,

        ord(configs["KEY_INSTUP"]):   event_instup,
        ord(configs["KEY_INSTDWN"]):  event_instdown,
        ord(configs["KEY_TS_DEC"]):   event_ts_dec,
        ord(configs["KEY_TS_INC"]):   event_ts_inc,
        ord(configs["KEY_DUR_DEC"]):  event_dur_dec,
        ord(configs["KEY_DUR_INC"]):  event_dur_inc,

        ord(configs["KEY_NOTE_INC"]): event_note_inc,
        ord(configs["KEY_NOTE_DEC"]): event_note_dec,

        # TODO: Let some keys be shortcuts for setting the time steps to certain values
        ord(configs["KEY_TIME_STEP_1"]): tsstep_1,
        ord(configs["KEY_TIME_STEP_2"]): tsstep_2,
        ord(configs["KEY_TIME_STEP_3"]): tsstep_3,

        # TODO: Function for setting duration directly
        # ord(configs["KEY_SET_DUR"]): event_note_dec,


        ord(configs["KEY_EVPLAY"]):  event_play,
        ord(configs["KEY_TOGPLAY"]): pattern_play
        }

### This defines the "keyboard to note" mapping.
play_keymaps = {
        ord(configs["KEY_C"]):  ".00",
        ord(configs["KEY_Cs"]): ".01",
        ord(configs["KEY_D"]):  ".02",
        ord(configs["KEY_Ds"]): ".03",
        ord(configs["KEY_E"]):  ".04",
        ord(configs["KEY_F"]):  ".05",
        ord(configs["KEY_Fs"]): ".06",
        ord(configs["KEY_G"]):  ".07",
        ord(configs["KEY_Gs"]): ".08",
        ord(configs["KEY_A"]):  ".09",
        ord(configs["KEY_As"]): ".10",
        ord(configs["KEY_B"]):  ".11",
        ord(configs["KEY_C2"]): ".00"
        }
# ord() returns Unicode integer equivalent of character (used by curses)

# The overwrite_nev_mode decides how new_event_value should be overwritten:
# LAST_INSERT - nev gets values from last event inserted
# ACTIVE_EVENT - nev gets values from the currently active / focused event
# OFF - nev is not overwritten
#overwrite_nev_mode = ?

# "nev" for short - Values to use when inserting new events
new_event_value = {
        "inst": int(configs["NEV_INST"]),
        "dur": float(configs["NEV_DUR"]),
        "start": float(configs["NEV_START"])
        }

### / SETUP ###

def draw_events(stdscr, pad, pattern):
    # TODO: Sort events before drawing to pad
    # pattern.events_list.sort(key=operator.attrgetter('start'))
    (max_y, max_x) = stdscr.getmaxyx()
    for ev_num, event in enumerate(pattern.events_list):
        if ev_num == pattern.active_event:
            pad.addstr(ev_num, 0, event.str(), curses.A_STANDOUT)
        else:
            pad.addstr(ev_num, 0, event.str())
    pad.refresh(0,0,0,0,max_y-2,max_x-2)

# CURSES
#def draw_ ..etc
