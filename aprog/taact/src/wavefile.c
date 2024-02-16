#include "wavefile.h"
#include <stdlib.h>

/* Implementation depends on whether to use libsndfile or not */

#ifndef INTERNALFM  
                        /***** Using libsndfile *****/
#include <sndfile.h>
int wave_write(struct wave16_t* w){
    if (w->length == 0){
        return 1;
    }
    struct SF_INFO sf_info = {
        .frames = (sf_count_t) w->length,
        .samplerate = w->samplerate,
        .channels = 1,
        .format = SF_FORMAT_WAV | SF_FORMAT_PCM_16,
    };
    SNDFILE* ofile = sf_open(w->name, SFM_WRITE, &sf_info);
    if (ofile == NULL){
        sf_close(ofile);
        return 0;
    }
    sf_write_short(ofile, w->array, w->length);
    sf_close(ofile);
    return 1;
}
int wave_read(struct wave16_t* w){
    SF_INFO sf_info;
    SNDFILE* ifile = sf_open(w->name, SFM_READ, &sf_info);
    if (ifile == NULL){
        sf_close(ifile);
        return 0;
    }
    w->length = sf_info.frames;
    w->array = malloc(w->length * sizeof(short));
    sf_read_short(ifile, w->array, w->length);
    sf_close(ifile);
    return 1;
}
#else
                /***** Using self-made wave file manager *****/
#include <string.h>
#include <stdint.h>
#include <file.h>

int wave_write(struct wave16_t* w){

}
int wave_read(struct wave16_t* w){
    
}

#endif
