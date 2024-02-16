#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "wavefile.h"


int wave_print_info(struct WAV* w);
int wave_valid(struct WAV* w);

/** mono_read()
 * Function that takes the name of a wav file and a pointer to a target 
 * mono_audio struct to be written to. The function opens the file and reads
 * the data that is expected to be the wav metadata. If the wav is not on a 
 * valid format, an error code is returned. If not, all the wav data will be 
 * read into the data field of the mono_audio struct.
 */
int mono_read(char* filename, struct mono_audio* a){
    FILE *ptr_wavfile;
    ptr_wavfile = fopen(filename, "rb");
    if (!ptr_wavfile){
        fprintf(stderr, "Unable to open the given file\n");
        return 0;
    }

    a->data = NULL; a->wavinfo = NULL;  /* Temporarily? if something goes wrong */
    /* The struct only contains a pointer, so if I'm not mistaken, you have to
     * do the actual initialization of the struct before reading into it. */

    /* Read metainformation */
    a->wavinfo = malloc(sizeof(struct WAV));
    fread(a->wavinfo, sizeof(struct WAV), 1, ptr_wavfile);
    if (!wave_valid(a->wavinfo)){
        fprintf(stderr, "File format is invalid / unsupported\n");
        return 0;
    }

    /* Read data samples  */
    a->data = malloc(a->wavinfo->sc2_size);
    fread(a->data, sizeof(int16_t), a->wavinfo->sc2_size/2, ptr_wavfile);

    fclose(ptr_wavfile);

    //fseek(ptr_wavfile, 4*sizeof(char), SEEK_SET);     // not needed when reading all data sequentially, I think. // Can be used to jump to the data subchunk
    //free(data);

    return 1;
}

int mono_write(char* filename, struct mono_audio* a){
    FILE *ptr_wavfile;
    ptr_wavfile = fopen(filename, "wb");
    if (!ptr_wavfile){
        return 0;
    }
    fwrite (a->wavinfo, sizeof(struct WAV), 1, ptr_wavfile);
    fwrite (a->data, a->wavinfo->sc2_size, 1, ptr_wavfile);
    fclose(ptr_wavfile);
    return 1;
}

int mono_free(struct mono_audio* a){
    if (a->data != NULL)
        free(a->data);
    if (a->wavinfo != NULL)
        free(a->wavinfo);
}

int main(int argc, char** argv){
    struct mono_audio snd;
    if (!mono_read("gtr.wav", &snd))
        exit(EXIT_FAILURE);

    wave_print_info(snd.wavinfo);
    mono_write("gituur.wav", &snd);
    mono_free(&snd);
    return 0;
}


/** wave_valid()
 * Check if the given wave 
 */
int wave_valid(struct WAV* w){
    char* expected = "RIFF";
    for (unsigned c = 0; c < 4; c++){
        if (w->chunk_id[c] != expected[c])
            return 0;
    }
    expected = "WAVE";
    for (unsigned c = 0; c < 4; c++){
        if (w->format[c] != expected[c])
            return 0;
    }
    expected = "fmt ";
    for (unsigned c = 0; c < 4; c++){
        if (w->sc1_id[c] != expected[c])
            return 0;
    }
    expected = "data";
    for (unsigned c = 0; c < 4; c++){
        if (w->sc2_id[c] != expected[c])
            return 0;
    }

    return 1; 
}

int wave_print_info(struct WAV* w){
    char to_print[5];
    to_print[4] = '\0';

    for (unsigned n = 0; n < 4; n++){
        to_print[n] = w->chunk_id[n];
    }
    printf("Chunk ID: \t%s\n", to_print);
    printf("Chunk size:\t%uKB\n", w->chunk_size >> 10);

    for (unsigned n = 0; n < 4; n++){
        to_print[n] = w->format[n];
    }
    printf("Format:\t\t%s\n", to_print);
    for (unsigned n = 0; n < 4; n++){
        to_print[n] = w->sc1_id[n];
    }
    printf("\nSubchunk1 ID:\t%s\n", to_print);
    printf("Subchunk1 size:\t%u\n", w->sc1_size);
    printf("Audio format:\t%u\n", w->audio_format);
    printf("Num. channels:\t%u\n", w->num_channels);
    printf("Sample rate:\t%u\n", w->sample_rate);
    printf("Byte rate:\t%u\n", w->byte_rate);
    printf("Block align:\t%u\n", w->block_align);
    printf("Bits / sample:\t%u\n", w->bits_per_sample);

    for (unsigned n = 0; n < 4; n++){
        to_print[n] = w->sc2_id[n];
    }
    printf("\nSubchunk2 ID:\t%s\n", to_print);
    printf("Subchunk2 size:\t%u\n", w->sc2_size);

    // TODO: Check that the format is right and return 0 if not
    return 1;
}
