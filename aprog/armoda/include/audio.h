#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <jack/jack.h>
#include <sndfile.h>


typedef jack_default_audio_sample_t sample_t;
int process(jack_nframes_t, void*);
void make_table();
void jack_setup();
void jack_shutdown();

typedef struct {
    sample_t ** signal;
    long samplerate;
    long length;
    int channels;
} sound;

void load_sample(char*, sound*);
void writeaudio(char *filename, double *signal, sf_count_t length, int Fs);
double *readaudio(char *filename, SF_INFO *soundInfo);

// We're declaring global variables in armoda.c and setting them in audio.c.
// These externs are here to tell audio.c that the global variables exist.
// Probably bad programming, but it will do for now.
extern jack_port_t *output_port;
extern jack_client_t *client;
extern sample_t *sample;
extern sample_t *time_table;
extern long sample_offset;
extern jack_nframes_t sample_nframes;  
extern sample_t *phasor;       
extern sample_t **filters;


#endif
