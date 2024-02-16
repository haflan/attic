#include "audio.h"

jack_port_t *output_port;
jack_client_t *client;
sample_t *sample;
long sample_offset = 0;
jack_nframes_t sample_nframes;  // Because mono, number of frames = number of samples (points)  in the sample (sound)
sample_t *phasor;       // This is a phasor that decides the speed at which to play the sample. 
                        // The idea is that the phasor can later be replaced by an arbitrary wave table describing
                        // how the sample should be read. This should make it easy to play it backwards, slower etc. 
sample_t *time_table;   // The 'time table' is made by manipulating the sample with the phasor.

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
        phasor[n] = (n % (sample_nframes / 3)) * phasor_step;
        printf("index[%d] = %ld\n", n, (long) (sample_nframes * phasor[n]));
        time_table[n] = sample[(long) (phasor[n] * sample_nframes)];
    }
    free(phasor);
    memcpy(sample, time_table, sizeof(sample_t) * sample_nframes);
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void jack_shutdown (void *arg)
{
	exit (1);
}

/* load a sample from sound file into a float table */
void load_sample(char *filename)
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

void jack_setup(int argc, char **argv) 
{
	const char **ports;
	const char *client_name = "simple";
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;
    sample = malloc(1024 * sizeof(sample_t));
    //for(int n = 0; n < 1024; n++){
    //    sample[n] = (sample_t) sin(6.28 * 100 / 1024);
    //}

    /* load sample file given as argument */
    if(argc > 0)
        load_sample("gtr.wav");
    else
        printf("No sample file name given.");

    /* make a 'time table' */
    make_table();

	/* open a client connection to the JACK server */

	client = jack_client_open (client_name, options, &status, server_name);
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

	/* display the current sample rate. */

	printf ("engine sample rate: %" PRIu32 "\n",
		jack_get_sample_rate (client));

	/* create a port */
	output_port = jack_port_register (client, "output",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

	/* Connect the ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */

	ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsInput);
	if (ports == NULL) {
		fprintf(stderr, "no physical playback ports\n");
		exit (1);
	}

	if (jack_connect (client, jack_port_name (output_port), ports[0])) {
		fprintf (stderr, "cannot connect output ports\n");
	}
	if (jack_connect (client, jack_port_name (output_port), ports[1])) {
		fprintf (stderr, "cannot connect output ports\n");
	}

	free (ports);
}
