#!/bin/python
### pynblinc setup file - loads config and sets up environment

# TODO: Read the keyboard mappings from the config file
import re
import pyncore
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

# TODO: Add these functions
edit_keymaps = {
        ord(configs["KEY_UP"]):   pyncore.event_up,  # add current_note (last played)
        ord(configs["KEY_DOWN"]): pyncore.event_down, # go to next event in list
        ord(configs["KEY_ADD"]):  pyncore.event_add,   # go to previous event
        }

## Editor settings (can be overwritten during runtime)
# Default Step sizes; number of beats to increase/decrease a value by when using
# keyboard to scroll through values.
step_size = {
        "inst": int(configs["STEP_INST"]),       # instrument
        "dur": float(configs["STEP_DUR"]),       # event duration
        "start": float(configs["STEP_START"])    # event start
        }


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
