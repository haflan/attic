#include <iostream>
#include <math.h>
#include "filemanagement.h"
#include "speechanalysis.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 3 || argc > 4)
	{
		cout << endl << " ---- Invalid use, read the readme.txt ----" << endl;
		return 0;
	}
	bool RELP = (argc == 4 ? true : false);		// If there's a third argument, use RELP

	SF_INFO soundInfo;
	double *sound = readaudio(argv[1], &soundInfo);

	unsigned int Fs = soundInfo.samplerate;
	unsigned int soundLength = (unsigned int) soundInfo.frames;
	unsigned int frameStep = (unsigned int) (0.02 * Fs);
	unsigned int winLengthLPC = (unsigned int) (0.03 * Fs);
	unsigned int winLengthPA = (unsigned int) (0.05 * Fs);
	unsigned int p = 14;
	unsigned int D = 4;
	double threshold = 0.5;

	unsigned int indexOffsetLPC = (winLengthPA - winLengthLPC) / 2;

	if (soundLength % frameStep != 0)
	{
		unsigned int framePadding = frameStep - (soundLength % frameStep);
		sound = zeroPadding(sound, soundLength, framePadding, PAD_BACK, true);
		soundLength += framePadding;
	}

	unsigned int windowPadding = (winLengthPA - frameStep) / 2;
	unsigned int numberOfFrames = soundLength / frameStep;

	/* ----- Zero padding ----- */
	sound = zeroPadding(sound, soundLength, windowPadding, PAD_FRONT, true);
	soundLength += windowPadding;	// Update soundLength before the next padding
	sound = zeroPadding(sound, soundLength, windowPadding, PAD_BACK, true);
	soundLength += windowPadding;	// Update soundLength
	/*--------------------------*/

	if (RELP)
	{
		bool hfRegenAlgorithm = (argv[3][0] == 's' ? false : true);	// Use HF Regeneration algorithm or simple zero insertion?

		double ** resSegments = new double *[numberOfFrames];
		Parameters * params = relpAnalysis(sound, soundLength, p, numberOfFrames, indexOffsetLPC, frameStep, winLengthLPC, resSegments, D);
		
		double * synthesized = relpSynthesis(resSegments, params, p, D, numberOfFrames, winLengthLPC, windowPadding, frameStep, hfRegenAlgorithm);
		
		normalize(synthesized, frameStep*numberOfFrames);	// Avoid clipping when saving the .wav
		writeaudio(argv[2], synthesized, frameStep*numberOfFrames, Fs);
		
		delete[] synthesized;
		delete[] params;
		deleteM(resSegments, numberOfFrames);
	}
	else
	{
		Parameters * params = analysis(sound, soundLength, p, threshold, numberOfFrames, indexOffsetLPC, frameStep, winLengthLPC, winLengthPA, Fs);

		double * synthesized = synthesis(p, numberOfFrames, frameStep, params);

		normalize(synthesized, frameStep*numberOfFrames);	// Avoid clipping when saving the .wav
		writeaudio(argv[2], synthesized, frameStep*numberOfFrames, Fs);

		delete[] synthesized; 
		delete[] params;

	}

	delete[] sound;
	return 0;
}
