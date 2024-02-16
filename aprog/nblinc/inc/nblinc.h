#pragma once

#include <stdbool.h>

/***** SETUP *****/
enum {PREV, NEXT};
// line number offset
#define LN_OFFSET 5
#define TS_OFFSET 10    // timestamp offset.


// initial defaults
//int default_note = 69;  //TODO: add option for displaying frequency, supercollider note etc.
//int default_dur = 0.5;

/* GLOBAL VARIABLES */
/* extern just means 'declare without defining, 
 * and is *only needed for (global) variables* that are defined globally,
 * in other words without a scope in other files. 
 * A locally defined variable is defined within a scope, which means that
 * it will not be accessed by an external file directly.
 */
/* view */
extern int curX;
extern int curY;
extern int yMax, xMax;
extern bool view_active;
extern char ** textbuffer;
/* events */
extern double tempo;
extern double step;
extern double long_step;
extern int num_events;
/* play state */ 
extern enum {NOPLAY, PLAY_NOTE, PLAY_LOOP, PLAY_SEQ} play_state;

/* nblinc interface events type */
typedef struct Event {
    double timestamp;
    unsigned char inst;
    unsigned char note;
    double amp;
    double dur;
    int ev_number;
    struct Event * next;
    struct Event * prev;
} Event;

/* global events */
extern Event *fe, *ce, *le, *ett;   // The event in focus
extern Event *etl;  /* event to loop from */

/***** FUNCTIONS *****/
void action_loop();   // the main one

/*** Actions ***/
void free_events();

/* view */
void toggle_view();
void print_events();
void print_view();
void print_line();
void redraw_all();

/* event list */
void insert_event();
void append_event();
void swap_events(bool next_event);
void focus_event(int jump);

/* for changing stuff */
void ch_timestamp(double ts_increase);
void ch_inst(bool next_instrument);
void ch_note(signed char note_increase);
void ch_amp(double amp_inc);
void ch_dur(double dur_increase);

/* default keybindings go here */
static const char INSERT = 'A';
static const char APPEND = 'A';
