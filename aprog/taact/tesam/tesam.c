/**
 * Demonstrates the general sequencer idea and how parts of it could be implemented
 */
#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include "sound.h"
#include "showcase.h"

/** Write to WAV using libsndfile **/
void write_wav(int16_t* wave_array, uint32_t wave_len) {
    struct SF_INFO sf_info = {
        .frames = wave_len,
        .samplerate = AUDIO_HZ,
        .channels = 1,
        .format = SF_FORMAT_WAV | SF_FORMAT_PCM_16,
    };
    SNDFILE* ofile = sf_open("simulated.wav", SFM_WRITE, &sf_info);
    // int16 is the same as short, so use the sf_write_short for this 
    sf_write_short(ofile, (const int16_t*) wave_array, wave_len);
    sf_close(ofile);
}

int main(int argv, char **argc)
{
    if (argv == 1) {
        sim_mode = "\0";
    } else {
        length_of_wav = atoi(argc[1]);
        sim_mode = "sweep\0";
    }
    if (argv > 3) {
        freq_s = atoi(argc[2]);
        freq_e = atoi(argc[3]);
    }
    empty_wave(length_of_wav);
    simulation(length_of_wav, sim_mode, freq_s, freq_e);
    write_wav(wave_samples, length_of_wav);
    printf("Wrote data to 'simulation.wav'\n");
    free(wave_samples);

	return 0;
}
