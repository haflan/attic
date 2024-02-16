#pragma once
#include <jack/jack.h>
#include <jack/midiport.h>
#include "nblinc.h"

void jack_init();
void toggle_play();
jack_client_t *client;
/* alternative to externs: make functions that just 
 * return the global variables. */

/* instead of these externs... */
extern jack_nframes_t samples_per_beat;
extern jack_nframes_t next_timestamp;
extern jack_nframes_t pos_frame;
extern const jack_position_t zero_pos;
/* use functions like this */
jack_nframes_t get_samples_per_beat();
jack_nframes_t get_transport_frame();


/* midi events */
typedef struct MidiEvent{
    jack_nframes_t ts;
    unsigned char note;
    unsigned char vel;
    unsigned char msg; 
    struct MidiEvent * off;
    struct MidiEvent * next;
    struct Event * nblink; /* linked nblinc event */
} MidiEvent;
/* This consists of the following:
 *    ts    =  timestamp
 *    note  =  midi note
 *    vel   =  velocity (0-127)
 *    msg   =  midi message:
 *      0x8# - note off for channel # (0-F).
 *      0x9# - note on for channel # (0-#).
 *      (http://www.onicos.com/staff/iz/formats/midi-event.html)
 *      For nblinc, the midi channel number should be given by
 *      the instrument. To make a note off event corresponding to
 *      a note on I suppose you just subtract 0x0F:
 *        note_off_event->msg = note_on_event->msg - 0x0F
 */

 /* set up global midi events used to control the list */
extern MidiEvent * fme;  /* first midi event in events list */
extern MidiEvent * metr; /* midi event to trigger next */

MidiEvent * make_midiseq(Event*);
void play_note(Event*);
void tog_play_loop(Event*);
void tog_play_seq(Event*);
void trigger_event(unsigned char*, MidiEvent*);
void kill_midiseq(MidiEvent*);
