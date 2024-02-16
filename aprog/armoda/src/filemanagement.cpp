#ifndef FILEMANAGEMET_H
#define FILEMANAGEMET_H

#include <iostream>
#include <sndfile.hh>

using namespace std;

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

	double *signal = new double[length];	// We want to read all frames of the signal.

	sf_readf_double(file, signal, length);

	sf_close(file);

	return signal;
}

#endif
