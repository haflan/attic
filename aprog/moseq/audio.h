#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <jack/jack.h>
#include <sndfile.h>

typedef jack_default_audio_sample_t sample_t;
// Not sure if these externs are needed. Probably not, if they're not used in moseq.c
extern jack_port_t *output_port;
extern jack_client_t *client;
extern sample_t *sample;
extern long sample_offset;
extern jack_nframes_t sample_nframes;  // Because mono, number of frames = number of samples (points)  in the sample (sound)
extern sample_t *phasor;       // This is a phasor that decides the speed at which to play the sample. 
                        // The idea is that the phasor can later be replaced by an arbitrary wave table describing
                        // how the sample should be read. This should make it easy to play it backwards, slower etc. 
extern sample_t *time_table;   // The 'time table' is made by manipulating the sample with the phasor.

// Most of these are probably unnecessary too, except jack_setup, which is called in moseq.c 
int process(jack_nframes_t, void*);
void make_table();
void load_sample(char*);
void jack_setup();

