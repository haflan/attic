/* Supply macro -DINTERNALFM to use internal wav file manager */
#ifndef _WAVEFILE_H_
#define _WAVEFILE_H_

/** struct WAV
 * This will always look like this for PCM coded wavs.
 * If you want to support more encodings, this must be done differently.
 */
struct WAV { 
    /* "RIFF" chunk descriptor */
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];         
    /* Subchunk 1 - fmt */
    char sc1_id[4];
    uint32_t sc1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    /* Subchunk 2 - data */
    char sc2_id[4];
    uint32_t sc2_size;
    /* Beware ^ This is the size in *bytes*, not the number of frames.
     * In other words, it is twice as big as the number of frames / samples */
};

/** struct mono_audio
 * Contains wav info in the form of a WAV struct, and a data array for
 * storing the actual sound sample data.
 */
struct mono_audio {
    struct WAV* wavinfo;
    void* data;
    unsigned len;   /* length */
    unsigned sr;    /* sample rate */
};

/* Read from and write to wave file */
int mono_read(char* filename, struct mono_audio*);
int mono_write(char* filename, struct mono_audio*);

/* Free memory allocated for the wav array */
int wave_free(struct mono_audio* w);
#endif
