#include "audio.h"

int process (jack_nframes_t nframes, void *arg)
{
	sample_t *out = (sample_t *) jack_port_get_buffer(output_port, nframes);
    //jack_position_t pos;
    //jack_nframes_t frames_left = nframes;
    if(sample_nframes - sample_offset > 2*nframes){
        memcpy(out, sample + sample_offset, sizeof(sample_t) * nframes);
        sample_offset += nframes;
    }
    else{
        memset(out, 0, sizeof(sample_t) * nframes);
        exit(1);
    }
    /* calculate rms of frame */
    float rms = 0.0;
    for(int n = 0; n < nframes; n++)
        rms += (float) out[n] * out[n];
    rms /= nframes;
    rms = sqrt(rms);
    printf("rms = %f\n", rms); 


    /*
    // If we have reached the end of the sample:
    if(sample_nframes - sample_offset < nframes){
        memcpy(out, sample + sample_offset, sizeof(sample_t) * (sample_nframes - sample_offset)); // Copy the rest of the samples to output buffer.
        frames_left -= sample_nframes + sample_offset; // Frames left = buffer size - index of last sample copied to output buffer + 1.
        memset(out + (nframes - frames_left), 0, sizeof(sample_t) * frames_left); // Set the remaining indices to 0.
        sample_offset += nframes + 1; // Just to make sure that sample_offset < sample_nframes.
    }
        // We want to play the sample once, ie. as long as the offset is less than the sample length:
    if(sample_offset < sample_nframes){
        memcpy(out + (nframes - frames_left), sample + sample_offset, sizeof(sample_t) * (nframes));
            // If we have already copied (sample_nframes - sample_offset) values into output buffer, we just have to fill the 
            // remaining (nframes - frames_left) indices. Therefore, we start to copy to the buffer at index (nframes - framesleft).
        sample_offset += nframes;
    }

                
   // for(int n = 0; n < nframes && sample_offset < sample_nframes; n++, sample_offset++){
   //     out[n] = sample[sample_offset];
   //     printf("%f", sample[sample_offset]);
   // }

	//memcpy (out, sample, sizeof (jack_default_audio_sample_t) * nframes);
    */

	return 0;      
}

void make_table()
{
    sample_t phasor_step = 1.0 / ((sample_t) sample_nframes);
    phasor = malloc(sizeof(sample_t) * sample_nframes); // For testing purposes, make the phasor as long as the sample. 
    time_table = malloc(sizeof(sample_t) * sample_nframes);
    for(int n = 0; n < sample_nframes; n++){
        phasor[n] = (n % (sample_nframes)) * phasor_step;
        //printf("index[%d] = %ld\n", n, (long) (sample_nframes * phasor[n]));
        time_table[n] = sample[(long) (phasor[n] * sample_nframes)];
    }
    free(phasor);
    memcpy(sample, time_table, sizeof(sample_t) * sample_nframes);
}

void jack_shutdown (void *arg)
{
	exit (1);
}

/* load a sample from sound file into a float table */
// TODO: Support stereofiles, eventually just read one channel from it.
void load_sample(char *filename, sound *input)
{
    SF_INFO soundInfo;
    SNDFILE* file;
    if((file = sf_open(filename, SFM_READ, &soundInfo)) == NULL)
    {
        printf("Error: Not able to open sample file '%s'.\n", filename);
        sf_close(file);
        exit(1);
    }

    sample_nframes = (jack_nframes_t) soundInfo.frames;
    sample = malloc(sample_nframes * sizeof(sample_t));
    sf_readf_float(file, sample, sample_nframes);
    sf_close(file);
}

void writeaudio(char *filename, double *signal, sf_count_t length, int Fs)
{
	SNDFILE *outfile; // *file;
	SF_INFO info = { length, Fs, 1, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 0, 0 };
	/* The format field in the above SF_INFO structure is made up of the bit-wise OR of a major format type (values between 0x10000 and 0x08000000), a minor format type (with values less than 0x10000) and an optional endian-ness value */

	if ((outfile = sf_open(filename, SFM_WRITE, &info)) == NULL)
	{ 
		printf("Error : Not able to open input file '%s'\n", filename);
		sf_close(outfile);
		exit(1);
	};

	sf_write_double(outfile, signal, length);

	sf_close(outfile);
}

double *readaudio(char *filename, SF_INFO *soundInfo)
{
	SNDFILE* file;	// New SNDFILE type
	
	if ((file = sf_open(filename, SFM_READ, soundInfo)) == NULL)
	{
		printf("Error : Not able to open input file '%s'\n", filename);
		sf_close(file);
		exit(1);
	}

	unsigned int length = (unsigned int) soundInfo->frames;

	double *signal = malloc(length*sizeof(double));	// We want to read all frames of the signal.

	sf_readf_double(file, signal, length);

	sf_close(file);

	return signal;
}
