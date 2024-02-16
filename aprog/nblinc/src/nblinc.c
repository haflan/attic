#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <csound/csound.h>

#include "nblinc.h"
#include "jackfunc.h"
/*
 *      nblinc = "Nearly Blind Composition"
 */

/*  GLOBAL VARIABLES    */
// This will later be used to connect the instrument letters to actual names.
char* instlist[] = {"piano", "violin", "testins"};
char* get_instrument(char** instlist, char insnum) { return instlist[insnum - 'A']; }
/* ncurses */
int curX = 1;
int curY = 0;
int yMax, xMax;

/*** default config ***/
double tempo = 140 / 60.0;   // 140 bpm placeholder
double step = 0.125;
double long_step = 0.125;
bool view_active = 1;
int num_events = 0;
char ** textbuffer;
unsigned int play_state = NOPLAY;
MidiEvent * midseq; /* TEMPORARY for testing with test_midfunc() */
Event * fe = NULL;   /* first event */
Event * ce = NULL;   /* currently chosen event */
Event * le = NULL;   /* last event */
Event * ae = NULL;   /* active event (last played) */
Event * ett = NULL;   /* event to trigger */
/* This is used to tell nblinc the position 
 * of the playback (if playing) */
/************************/

int main(int argv, char** argc){
    /*** init ***/
    /* ncurses */
    initscr();
    noecho();
    cbreak();                      
    timeout(1000);  /* wait only 1 second for input before continueing */
    /* to use this the code must be rewritten to use ncurses smart
     * drawing features instead of just redrawing the entire standard screen.
     *
    https://stackoverflow.com/questions/7772341/how-to-get-a-character-from-stdin-without-waiting-for-user-to-put-it
    */
    getmaxyx(stdscr, yMax, xMax);
    
    /* events */
    append_event();     // gotta have that first
    refresh();
    jack_init();

    action_loop();

    kill_midiseq(midseq);
	jack_client_close (client);
    save_sco(fe);
    free_events();
    endwin();                  

	exit (0);
}

/* unit test of make_midiseq() */
#include <stdio.h>
void test_midfunc(){
    midseq = make_midiseq(fe);
    MidiEvent * evtrav = midseq;
    FILE *fp;
    fp = fopen("./midevents.txt", "w+");
    while(evtrav != NULL){
        fprintf(fp, "at frame %" PRIu32 "\n", evtrav->ts);
        fprintf(fp, "[%d, %d, 0x%x]\n", evtrav->note, evtrav->vel, evtrav->msg);
        evtrav = evtrav->next;
    }
    fclose(fp);
    /* now try playing the sequence */
    if (play_state == NOPLAY){
        play_state = PLAY_SEQ;
        set_metr(midseq);
        set_fme(midseq);
        jack_transport_reposition(client, &zero_pos);
        jack_transport_start(client);
    }
}

void action_loop(){
    print_line();
    refresh();
    char num;
    bool running = 1;
    while (running){
        num = getch();
        switch(num){
            /* Flip through existing events */
            case 'w':   focus_event(1);         break;
            case 'b':   focus_event(-1);        break;
            case 'W':   focus_event(5);         break;
            case 'B':   focus_event(-5);        break;
            /* Add new events */
            /* insert event at same timestamp as current */
            case ',':   insert_event();         break;
            /* 10 = enter button. this should insert event at timestamp += last dur */
            case 'a':   insert_event();         break;
            case 'A':   append_event();         break;
            case 10:    append_event();         break;
            /* Changing instrument */
            case 'z':   ch_inst(NEXT);          break;
            case 'Z':   ch_inst(PREV);          break;
            /* Changing note, duration, position */
            case 'n':   ch_note(-1);            break;
            case 'N':   ch_note(-12);           break;
            case 'e':   ch_note(1);             break;
            case 'E':   ch_note(12);            break;
            /* Change duration */
            case 'h':   ch_dur(-step);          break;
            case 'i':   ch_dur(step);           break;
            case 'H':   ch_dur(-1.0);           break;
            case 'I':   ch_dur(1.0);            break;
            /* Change timestamp (position in time) */
            case 's':   ch_timestamp(-step);    break;
            case 't':   ch_timestamp(step);     break;
            case 'S':   ch_timestamp(-1.0);     break;
            case 'T':   ch_timestamp(1.0);      break;
            //case 'p':   toggle_play();          break;
            case 'v':   toggle_view();          break;
            case ' ':   test_midfunc();          break;
            case 27:    running = 0;            break; // esc button
            default:    break;
        }
        redraw_all();
        //if(view_active)
        //    print_view();
        move(curY, TS_OFFSET);
    }
}
