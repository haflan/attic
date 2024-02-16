#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "wavefile.h"

#define WAV_TXT_LEN 4
#define NOISY_TXT_LEN 6


void std_error(char* cmdname){
    fprintf(stderr, 
            "Usage:\t%s <input.wav>|<directory> [-r] [-o <output.wav>] [-s <SNR value>]\n", 
            cmdname);
}


void wave_print_info(struct wave16_t*);
void wave_reverse(struct wave16_t*, struct wave16_t*);
void wave_copy(struct wave16_t*, struct wave16_t*);


int main(int argc, char** argv){
    int opt;
    int recursive = 0;
    char* output_file = NULL;
    int output_file_given = 0;
    char* input_file = NULL;
    double snr = 50.0;
    while ((opt = getopt(argc, argv, "ro:O:s:")) != -1){
        switch(opt) {
        case 'r':
        case 'R':
            recursive = 1;
            break;
        case 'o':
            output_file = optarg;
            output_file_given = 1;
            break;
        case 'O':
            output_file = optarg;
            break;
        case 's':
            snr = atof(optarg);
            break;
        default:
            goto failure;
        }
    }

    if (optind >= argc) {
        printf("%s: no input file given.\n", argv[0]);
        goto failure;
    }

    input_file = argv[optind];

    /* If no output file name is given, simply use 
     *          <input_name w/o extension>-noisy.wav */
    if (!output_file_given){
        unsigned inlen = strlen(input_file);
        output_file = malloc((inlen + NOISY_TXT_LEN)*sizeof(char));
        output_file = strncpy(output_file, input_file, inlen - WAV_TXT_LEN);
        output_file = strcat(output_file, "-noisy.wav");
    }

    struct wave16_t input_wav = { .name = input_file, .samplerate = 16000, .length=0};
    struct wave16_t output_wav = { .name = output_file, .samplerate = 16000, .length=0};
    if (!wave_read(&input_wav)){
        printf("Reading '%s' failed. Does it really exist?\n", input_wav.name);
        goto failure;
    }

    //wave_reverse(&input_wav, &output_wav);
    wave_print_info(&input_wav);

    wave_reverse(&input_wav, &output_wav);

    wave_print_info(&output_wav);

    if (!wave_write(&output_wav)){
        printf("Writing '%s' failed. \n", output_wav.name);
        goto failure;
    }

    wave_free(&input_wav);
    wave_free(&output_wav);

    /* I'm not sure of any alternative way of doing this */
    if (!output_file_given)
        free(output_file);

    exit(EXIT_SUCCESS);

failure:
    std_error(argv[0]);
    exit(EXIT_FAILURE);
}

void wave_copy(struct wave16_t* in, struct wave16_t* out){
    if (out->length != in->length){
        wave_free(out);
        out->array = malloc(in->length * sizeof(short));
        out->length = in->length;
    }
    for (unsigned n; n < in->length; n++){
        out->array[n] = in->array[n];
    }
}

void wave_reverse(struct wave16_t* in, struct wave16_t* out){
    if (out->length != in->length){
        wave_free(out);     /* Just overwrite existing data*/
        out->array = malloc(in->length * sizeof(short));
        out->length = in->length;
    }
    for (unsigned n; n < in->length; n++){
        out->array[n] = in->array[in->length - 1 - n];
    }
}

int wave_free(struct wave16_t* w){
    if (w->array != NULL){
        free(w->array);
        return 1;
    }
    return 0;
}

void wave_print_info(struct wave16_t* wav){
    unsigned long rms = 0;
    for (unsigned n = 0; n < wav->length; n++){
        rms += (wav->array[n])*(wav->array[n]);
    }
    rms = wav->length;
    rms = sqrt(rms);
    //printf("wav_name: %s\nwav_length: %d\nwav_srate: %d\nrms: %lu\n", 
    //        wav->name, wav->length, wav->samplerate, rms);
    printf("wav_name: %s\nwav_length: %d\nrms: %lu\n", wav->name, wav->length, rms);
}
