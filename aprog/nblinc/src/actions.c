#include <stdlib.h>
#include <ncurses.h>
#include "nblinc.h"
#include <stdio.h>

void set_state(unsigned new_state){
    
}
/* for changing event properties */
void ch_timestamp(double ts_inc){
    double new_ts = ce->timestamp + ts_inc;
    /* make sure the new time stamp is not below 0 */
    ce->timestamp = (new_ts < 0.0 ? 0.0: new_ts);    // don't go below 0.0
    /* if timestamp is smaller than the timestamp of previous event */
    while(ce->prev != NULL && ce->timestamp < ce->prev->timestamp){
        swap_events(PREV);
        curY--;
    }
    /* if timestamp is larger than the timestamp of previous event */
    while(ce->next != NULL && 
            ce->timestamp > ce->next->timestamp){
        swap_events(NEXT);
        curY++;
    }
}
void ch_inst(bool next){
    if(next && ce->inst < 0x0F)
        ce->inst++;
    else if(!next && ce->inst > 0x00)
        ce->inst--;
}
void ch_note(signed char note_inc){
    ce->note += note_inc;
    ce->note = (ce->note < 0 ? 0 : ce->note);
    ce->note = (ce->note > 127 ? 127 : ce->note);
}
void ch_dur(double dur_inc){
    ce->dur += dur_inc;
    ce->dur = (ce->dur < step ? step : ce->dur);
}
void ch_amp(double amp_inc){
    ce->amp += amp_inc;
    ce->amp = (ce->amp < 0.0 ? 0.0 : ce->amp);
    ce->amp = (ce->amp > 1.0 ? 1.0 : ce->amp);
}

/* manipulating nblinc events */
void insert_event(){
    /* Insert in middle of list
     * This requires some swapping,
     * which is *not* necessary in 
     * append_event()               */
    if(ce == le)
        append_event(ce);
    else{
        curY++;
        Event * prev_ev = ce;
        Event * pe_next = prev_ev->next;
        Event * new_event = malloc(sizeof(Event));
        
        /* might wanna clean this up */
        prev_ev->next = new_event;
        pe_next->prev = new_event;
        new_event->next = pe_next;
        new_event->prev = prev_ev;

        new_event->inst       = le->inst;
        new_event->timestamp  = ce->timestamp;
        new_event->note       = le->note;
        new_event->dur        = le->dur;
        new_event->ev_number  = num_events;
        ce = new_event;
        num_events++;
    }
}
void append_event(){
    curX = 1;
    curY = num_events;

    Event * new_event = malloc(sizeof(Event));
    new_event->next = NULL;
    /* if this is the first element: */
    if(le == NULL){
        new_event->prev = NULL;
        fe = new_event;
        new_event->inst       = 0x00;
        new_event->timestamp  = 0.0;
        new_event->note       = 60;
        new_event->amp        = 0.5;
        new_event->dur        = 0.5;
        new_event->ev_number  = 0;
    }
    else{
        le->next      = new_event;
        new_event->prev       = le;
        new_event->inst       = le->inst;
        new_event->timestamp  = le->dur+le->timestamp;
        new_event->note       = le->note;
        new_event->amp        = le->amp;
        new_event->dur        = le->dur;
        new_event->ev_number  = num_events;
    }
    le = new_event;
    num_events++;
    ce = new_event;
    /* set the actual values */
}
void free_events(){
    Event * this_event = fe;
    while(this_event != NULL){
        Event * next_event = this_event->next;
        free(this_event);
        this_event = next_event;
    }
}
void focus_event(int jump){
    if(jump > 0 && ce != le){
        ce = ce-> next;
        curY++;
    }
    if(jump < 0 && ce != fe){
        ce = ce->prev;
        curY--;
    }
}
void swap_events(bool next){
    Event * ne = ce->next;
    Event * pe = ce->prev;
    if(next && ce != le){
        if(ce == fe) { fe = ne; }
        else         { pe->next = ne; }
        if(ne == le) { le = ce; }
        else         { ne->next->prev = ce; }
        ne->prev = ce->prev;
        ce->next = ne->next;
        ce->prev = ne;
        ne->next = ce;
    }
    else if(!next && ce != fe){
        if(pe == fe) { fe = ce; }
        else         { pe->prev->next = ce; }
        if(ce == le) { le = pe; }
        else         { ne->prev = pe; }
        pe->next = ce->next;
        ce->prev = pe->prev;
        ce->next = pe;
        pe->prev = ce;
    }
}
/*  OOPS, no function overloading in C
 *  This should replace the other one later though.
 *
void focus_event(Event * new_current){
    ce = new_current;
    // redraw(), this should move cursor to
    // ce->ev_number
    // assuming ev_number is the number in the
    // lili order (WHICH IT IS CURRENTLY NOT)

} */

/* printing stuff */
void print_line(){
        Event * ev = ce;
        mvprintw(curY, curX, "  %9.4f: %c(%d, %1.3f)", ev->timestamp, ev->inst, ev->note, ev->dur);
        move(curY, TS_OFFSET);
}
/* Replace this one with a function that doesn't depend on JACK 
void print_view(){
    mvprintw(yMax - 9, 1, "- Entry %d / %d -\n    at time: %1.3f\n       note: %d\n   duration: %1.3f\n", 
            ce->ev_number, num_events, ce->timestamp, ce->note, ce->dur);
    mvprintw(yMax - 3, 1,  "- Info -\n first event: %d\n time: %d" PRIu32 "\n", fe->ev_number,
            (double) pos_frame / samples_per_beat, next_timestamp);
} */
void redraw_all(){
    clear();
    Event * ev = fe;
    move(0, 0);
    while(ev != NULL){
        if(play_state != NOPLAY && ev == ett)
            printw("->");
        else printw("  ");
        printw("%9.4f: %x(%d, %1.3f)\n", ev->inst, ev->timestamp, ev->note, ev->dur);
        ev = ev->next;
    }
    //printw("%" PRIu32 "\n", next_timestamp);
    move(curY, curX);
}
void toggle_view(){
    clear();
    view_active = !view_active;
}
char * format_bar(double total_beat, int beats_per_bar){
    char * bar_format;
    int bar = (int) total_beat / beats_per_bar;
    double beat = (int) total_beat % beats_per_bar;
    /* format the text string here*/
    return bar_format;
}

/* saving and loading files */
void save_sco(Event * countev){
    FILE * fp = fopen("./newseq.sco", "w+");
    while (countev != NULL){
        fprintf(fp, "i %d %9.5f %9.5f %9.5f %4d \n", 
                countev->inst+1, countev->timestamp, countev->dur,
                countev->amp, countev->note); 
        countev = countev->next;
    }
    fclose(fp);
}
