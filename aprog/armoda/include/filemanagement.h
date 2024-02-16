#include <iostream>
#include <math.h>
#include "sndfile.hh"

void writeaudio(char *filename, double *signal, sf_count_t length, int Fs);
double *readaudio(char *filename, SF_INFO *soundInfo);