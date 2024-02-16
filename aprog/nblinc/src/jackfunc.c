#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* for including global variables */
#include "jackfunc.h"

/* this is only used by jack functions, so no reason to make them external */
jack_port_t *output_port;
jack_nframes_t sample_rate;
jack_nframes_t samples_per_beat;
jack_position_t transport_pos;
jack_nframes_t pos_frame;
jack_nframes_t ntclock;
const jack_position_t zero_pos = { .frame = 0 };
jack_transport_state_t ts; 
bool overwrite_note = false;

MidiEvent * fme = NULL;
MidiEvent * metr = NULL;

int process (jack_nframes_t nframes, void *arg)
{
    int i; 
    /* MIDI buffer */
    void* port_buf = jack_port_get_buffer(output_port, nframes);
    unsigned char* buffer;
    jack_midi_clear_buffer(port_buf);
    switch(play_state)
    {
        case NOPLAY:    break;
            /* if the sequencer is not in play mode there's nothing to do */

        case PLAY_NOTE:
            /* This should play the next event using the ntclock */

            /* overwrite == true means that a new note was triggered before the
             * previous was finished. we then need to:
             * 1.   trigger note off instantly 
             * 2.   reset the ntclock 
             * 3.   set the new note to be played equal to the currently chosen
             * */
            /* UNCOMMENT WHEN TIME
            if(overwrite_note){
                buffer = jack_midi_event_reserve(port_buf, 0, 3);
                trigger_event(buffer, metr);
                ntclock = cme->ts;
                metr = cme;
                overwrite_note = false;
            }
            for(i = 0; i < nframes; i++){
                if(metr->ts == ntclock + i)
                    buffer = jack_midi_event_reserve(port_buf, i, 3);
                    trigger_event(buffer, metr);
            }
            // if the linked 'off' event is NULL, this must have been a note off
            if(metr->off == NULL)
                play_state = NOPLAY;
            // else, if the event is linked to an off event it must be a 'note on'
            else {
                metr = metr->off;
                ntclock += nframes;
            }
            */
            break;

        case PLAY_LOOP:
            /*
            // this should also use the (non-transport) ntclock
            for(i = 0; i < nframes; i++){
                if(metr->ts == ntclock + i)
                             
            }
            if(metr->link == NULL)
                
            else {
                metr = metr->next;
                ntclock += nframes;
            }*/
            break;

        case PLAY_SEQ:
            /* Play the sequence from start to finish using JACK transport */
            ts = jack_transport_query(client, &transport_pos);
            // the number of frames from the transport started to nframes[0]
            pos_frame = transport_pos.frame;    
            for (i = 0; i < nframes; i++){
                // play all events with timestamp equal to pos_frame + i
                while(metr->ts == pos_frame + i){
                    buffer = jack_midi_event_reserve(port_buf, i, 3);
                    trigger_event(buffer, metr);
                    // stop playing if we've reached the end. 
                    if (metr->next == NULL){
                        play_state = NOPLAY;
                        metr = fme;
                    }
                        
                    // if not, set metr to the next event in the midi events list
                    else
                        metr = metr->next;
                }
            }
            break;
        default:
            break;
    }
	return 0;      
}

/* use this to tell the process() what's going on */
void play_note(Event * nbevent){
    /* This plays a note unless the transport is running */
    /* - Set metr = current_event;
     * - Set ntclock = metr->timestamp?
     *   Let this be a ntclock that is used independently of JACK transpose.
     *   Update the ntclock on each process()  
     * - When the ntclock == metr->timestamp, trigger the 'note on'
     *   and set metr = metr->linked, so that the 'note off'
     *   event is the next to be triggered.
     */
    /* if a note is already playing stop it asap */
    if (play_state == PLAY_NOTE){
        overwrite_note = true;
    }
    else if (play_state == PLAY_LOOP)
    {
        //ntclock = cme->ts;
        //metr = cme;
    
    }
    play_state = PLAY_NOTE;

}
void tog_play_loop(Event * ev_to_loop_from){
    /* if the transport is rolling, tog_play_loop 
     * will stop it before starting the looping */
    fme = make_midiseq(ev_to_loop_from);
    if (ts == JackTransportRolling)
        tog_play_seq(NULL);
    /*  */
    if (ntclock == 0){
        /* ntclock starts at the timestamp of the event to loop from */
        //ntclock = melf->ts;   
        //metr = melf;    /* midi event to trigger next = midi event to loop from */
        //play_state = PLAY_LOOP;
    }
}
void tog_play_seq(Event * fnbevent){
    /* toggles playing of the entire sequence from start 
     * to finish, using the transport */
    /* this can possibly replace the toggle_play below */
    if (ts == JackTransportRolling){
        play_state = NOPLAY;
        jack_transport_stop(client);
        /* note off all the remaining notes */
        while(metr != NULL){
            /* trigger note off events (msg < 0x90) */
            if(metr->msg < 0x90){

            }
            metr = metr->next;
        }
    } else if (ts == JackTransportStopped){
        fme = make_midiseq(fnbevent);
        play_state = PLAY_SEQ;
        jack_nframes_t pos_frame = transport_pos.frame;
        jack_transport_start(client);
    }
    metr = fme;
    jack_transport_reposition(client, &zero_pos);
}
void set_fme(MidiEvent * toset){ fme = toset; } 
void set_metr(MidiEvent * toset){ metr = toset; } 
/* ^ used by void test_midfunc() in nblinc.c*/

#include <stdio.h>
void trigger_event(unsigned char * buffer, MidiEvent * me){
    FILE *fp = fopen("./played.txt", "a+");
    fprintf(fp, "%" PRIu32 ": %d, %d, 0x%x\n", me->ts, me->vel, me->note, me->msg);
    /* later you might change the data_size from 3 to 4
     * and use the 4th to set the instrument of the event */
    buffer[2] = me->vel;
    buffer[1] = me->note;
    buffer[0] = me->msg;
    fclose(fp);
}

/* make entire midi sequence from nblinc sequence */
/* realtime editing of midi sequence can be implemented later */
MidiEvent * make_midiseq(Event * fnbevent){
    /* returns pointer to the first midi event */
    Event * nbevent = fnbevent; /* nblinc event */
    MidiEvent * fmevent;        /* first midi event in playlist */
    /* create that first event (if nblinc sequence is not empty) */
    if (nbevent == NULL)
        /* empty nblinc sequence = empty midi sequence */
        return NULL;

    /* start placing note on and off events in the list */
    MidiEvent * prev_mevent;
    bool first_event = true;
    while(nbevent != NULL){
    /********* THIS IS GOOD (checked) *********/
        /*** Initialize the events ***/
        MidiEvent * noteon = malloc(sizeof(MidiEvent));
        MidiEvent * noteoff = malloc(sizeof(MidiEvent));
        /* note on event*/
        noteon->ts     = (jack_nframes_t) samples_per_beat * nbevent->timestamp;
        noteon->note   = nbevent->note;
        noteon->vel    = (unsigned char) 127 * nbevent->amp; /* assuming 0 < amplitude < 1.0 */
        noteon->msg    = 0x90 + nbevent->inst;      /* assuming 0x00 < instrument < 0x0F */
        noteon->off    = noteoff;
        noteon->nblink = nbevent;
        /* note off event*/
        noteoff->ts     = (jack_nframes_t) samples_per_beat * (nbevent->timestamp + nbevent->dur);
        noteoff->note   = nbevent->note;
        noteoff->vel    = (unsigned char) 127 * nbevent->amp; /* assuming 0 < amplitude < 1.0 */
        noteoff->msg    = 0x80 + nbevent->inst; /* assuming 0x00 < instrument < 0x0F */
        noteoff->off    = NULL;    /* note off points nowhere */
        noteoff->nblink = NULL;

    /********* THIS IS ALMOST GOOD *********/
    /* But there's something wrong either here or in the PLAY_SEQ part of process() */
        /*** Placing the events in the list ***/
        /* if this is the first midi event placed, noteoff will follow directly */
        if (first_event){
            fmevent = noteon;
            fmevent->next = noteoff;
            noteoff->next = NULL;   /* this must be the last event */
            first_event = false;
        } /* if not, the correct position in the list must be found for note on and off*/
        else {
            MidiEvent * traverse_mevent = prev_mevent;
            // new noteon is placed *after* traverse_mevent
            while (traverse_mevent->next != NULL && noteon->ts > traverse_mevent->next->ts) {
                traverse_mevent = traverse_mevent->next;
            }
            noteon->next            = traverse_mevent->next;
            traverse_mevent->next   = noteon;
            traverse_mevent         = noteon;
            // new noteoff is placed *after* noteon
            while (traverse_mevent->next != NULL && noteoff->ts > traverse_mevent->next->ts) {
                traverse_mevent = traverse_mevent->next;
            }
            noteoff->next           = traverse_mevent->next;
            traverse_mevent->next   = noteoff;
        }
        prev_mevent = noteon;   // keep this event for next iteration
        nbevent = nbevent->next;
    }
    return fmevent;
}
void kill_midiseq(MidiEvent * fmevent){
    MidiEvent * this_mevent = fmevent;
    MidiEvent * next_mevent = NULL;
    while(this_mevent != NULL){
        next_mevent = this_mevent->next;
        free(this_mevent);
        this_mevent = next_mevent;
    }
}


/*************************************************************/
/*******                                              ********/
/*******   THIS IS FINE. DON'T CHANGE ANYTHING HERE   ********/
/*******                                              ********/
/*************************************************************/

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void jack_shutdown (void *arg) {
	exit (1);
}

/* this should be called before the action_loop() */
void jack_init() {
	const char *client_name;
	jack_options_t options = JackNullOption;
	jack_status_t status;

    client_name = strrchr("nblinc", '/');
    if (client_name == 0) {
        client_name = "nblinc";
    } else {
        client_name++;
    }

	/* open a client connection to the JACK server */

	client = jack_client_open (client_name, options, &status);
	if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}

	/* tell the JACK server to call `process()' whenever
	   there is work to be done.
	*/

	jack_set_process_callback (client, process, 0);

	/* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, jack_shutdown, 0);

	/* display the current sample rate. 
	 */

	/* Delete if everything's working:
     * printf ("engine sample rate: %" PRIu32 "\n",
		jack_get_sample_rate (client));     */
    sample_rate = jack_get_sample_rate(client);
    samples_per_beat = sample_rate / tempo;

	/* create port ports */
	output_port = jack_port_register (client, "output",
					  JACK_DEFAULT_MIDI_TYPE,
					  JackPortIsOutput, 0);

	if (output_port == NULL) {
		fprintf(stderr, "no more JACK ports available\n");
		exit (1);
	}

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */
	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

	/* keep running until the transport stops
	 *while (client_state != Exit) {
     *    jack_nframes_t pos_frame = transport_pos.frame;
     *    printf("t = %" PRIu32 "\n", pos_frame);
	 *	sleep (1);
	 *}
     */
}

