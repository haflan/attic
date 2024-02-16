#!/bin/python
"""
Script for converting from MIDI to textual representation (for instance Csound)
"""

import sys
import mido
import re
import Pattern
import Events

midifile_folder = "/home/vh/sonic/melodies/"

# If I'm not mistaken, the `time` attribute of MIDI messages are the number of
# ticks since the last event.
# http://mido.readthedocs.io/en/latest/messages.html#converting-from-bytes
#

#def m2c(): midi to pch

def midi_to_pattern(midi_file, csperformancethread, verbose=False):
    # Get the title and mido of the file
    is_midi  = re.search("(.+)\.mid", midi_file)
    if not is_midi:
        raise Exception("{} is not a MIDI file".format(midi_file))
    melo_string  = is_midi.group(1)
    melo_string += "\n" + len(melo_string)*"="
    new_mido = mido.MidiFile(midi_file)

    tpb = new_mido.ticks_per_beat
    if verbose:
        print(tpb)

    new_pattern = Pattern.Pattern(csperformancethread)
    # Start converting to text string
    for track in new_mido.tracks:
        #TODO: check if the unnamed track is actually empty!
        #      if not empty, simply name it "unnamed track <#>" and include it
        if not track.name:
            if verbose:
                print("unnamed track ignored")
            continue
        if len(track) < 3:
            if verbose:
                print("empty track ignored")
            continue

        melo_string += "\n" + track.name + "\n" + "-"*len(track.name) + "\n"
        # TODO: On note_on - iterate through until you find the
        # corresponding note_off, and count total delta time between them.
        # This is done, but needs testing.
        if verbose:
            print("printed track. press for next")
        total_time = 0
        # FIXME uglyyy
        for mn, msg in enumerate(track):
            msg_data = msg.dict()
            dur_ticks = 0
            if msg_data['type'] == 'note_on':
                total_time += msg_data['time']
                current_note = msg_data['note']
                dur_ticks    = 0
                # Search for corresponding note off
                for i in range(mn, len(track)):
                    msg_data = track[i].dict()
                    dur_ticks += msg_data['time']
                    if msg_data['type'] == 'note_off' and msg_data['note'] == current_note:
                        break
                new_event = Events.Event([msg_data['channel']+1, total_time/tpb, dur_ticks/tpb, current_note])
                new_pattern.add_event(new_event)
    return new_pattern

def midi_to_text(ifile):
    # for now
    return midi_to_pattern(ifile)

def load_midi(midi_file):
    # Reads a MIDI file into an events list
    is_midi  = re.search("(.+)\.mid", midi_file)
    # First, check that a valid input file is given
    if not is_midi:
        raise Exception("{} is not a MIDI file".format(midi_file))
    new_mido = mido.MidiFile(midi_file)

def main():
    # Function for using the script as a standalone MIDI to text converter
    input_files = sys.argv[1:]

    if not input_files:
        print("No input files given")
        exit(1)

    for ifile in input_files:
        try:
            melo_string = midi_to_text(ifile)
            melo_string.print()
            print("Converted {} to text file.".format(ifile))
        except Exception as e:
            print(e)


if __name__ == "__main__":
    main()
