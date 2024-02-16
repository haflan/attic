#include <math.h>
#include <time.h>	
#include "speechanalysis.h"
#include <iostream>


#define pi 3.14159265359



Parameters::Parameters(void)
{
	setAll(0, NULL, 0, 0);
}
Parameters::~Parameters(void)
{
	delete[] coeffs;
}
void Parameters::setAll(unsigned int order, double * coeffs, unsigned int period, double gain)
{
	this->order = order;
	this->coeffs = coeffs;
	this->period = period;
	this->gain = gain;
}
void Parameters::setCoeffs(unsigned int order, double * coeffs)
{
	this->order = order;
	this->coeffs = coeffs;
}
void Parameters::setPeriod(unsigned int period)
{
	this->period = period;
}
void Parameters::setGain(double gain)
{ 
	this->gain = gain; 
}

void deleteM(double ** multiarray, unsigned int rows)
{
	// Really a memory management function, but we put it here
	for (unsigned int i = 0; i < rows; i++)
	{
		delete[] multiarray[i];
	}
	delete multiarray;
}

/* ----- Assorted (DSP) functions ----- */
double * acorr(double *signal, int signalLength, int maxLag)
{
	double *acf = new double[maxLag + 1];				// Init autocorrelation
	double *x = zeroPadding(signal, signalLength, maxLag + 1, PAD_BACK, false);

	for (int l = 0; l <= maxLag; l++)		// from l=0 to l=maxLag
	{
		acf[l] = 0;
		for (int n = 0; n < signalLength; n++)
		{
			acf[l] += x[n] * x[n + l];
		}
	}

	delete[] x;
	return acf;
}
double *zeroPadding(double *signal, int signalLength, int numberOfZeros, PadPosition position, bool override)
{
	double * newSignal = new double[signalLength + numberOfZeros];

	if (position == PAD_FRONT)
	{
		for (int n = 0; n < numberOfZeros; n++)
			newSignal[n] = 0;
		for (int n = 0; n < signalLength; n++)
			newSignal[numberOfZeros + n] = signal[n];
	}
	else
	{
		for (int n = 0; n < signalLength; n++)
			newSignal[n] = signal[n];
		for (int n = 0; n < numberOfZeros; n++)
			newSignal[signalLength + n] = 0;
		return newSignal;
	}
	if (override)	// If the new signal is replacing the old
		delete[] signal;

	return newSignal;
}
double *filter(double *B, double *A, double *x, int q, int p, int signalLength) 
{	// FIX THE INDEXING 

	/*
	B - Feed-forward coefficients
	q - FF order
	A - Feedback coefficients
	p - FB order
	*/

		/*	Unit testing confirms that this function works like in Matlab, except this one has an implicit 1 as the first element in A
			This is practical because AR-filtering coefficients have the format A(z) = 1 + ... */

	if (B == nullptr)
	{
		double one[1] = { 1 };
		B = one;
		q = 0;
	}
	else
		q--;	// Easy fix for indexing and readability

	double *y = zeros(signalLength);
	for (int n = 0; n < signalLength; n++)
	{
		int M = ((q < n) ? q : n);		// If the current n is less than q, we evaluate for k=0..n :	(B[0]*x[n-0]  to  B[n]*x[n-n])
		for (int k = 0; k <= M; k++)
		{
			y[n] += B[k] * x[n - k]; //cout << k;
		}

		int N = ((p < n) ? p : n);	// If the current n is less than p, we evaluate for k=1..n :		(A[0]*x[n-1]  to  A[n-1]*x[0]). Else if n >= p, we evaluate from k=1..p	 (A[0]*x[p-1]  to  A[n-1]*x[n-p])
		for (int k = 1; k <= N; k++)
			y[n] -= A[k - 1] * y[n - k];		// Fortegn avhengig av form på A[]. Dette bestemmes av koeffisientene som findCoeffs returnerer
	}
	return y;
}
double *zeros(int length)
{
	double *newSignal = new double[length];
	for (int n = 0; n < length; n++)
		newSignal[n] = 0;
	return newSignal;
}
double * addOne(double * coeffs, unsigned int p)
{
	double * added = new double[p + 1];
	added[0] = 1.0;
	for (unsigned int i = 0; i < p; i++)
	{
		added[i + 1] = coeffs[i];
	}
	return added;
}
double maxabs(double *signal, int signalLength)
{
	double max = 0;
	for (int n = 0; n < signalLength; n++)
	{
		if (abs(signal[n]) > max)
		{
			max = abs(signal[n]);
		}
	}
	return max;
}
void normalize(double *signal, int signalLength)
{
	double factor = 1 / maxabs(signal, signalLength);
	for (int n = 0; n < signalLength; n++)
	{
		signal[n] *= factor;
	}
}
/* ------------------------------------ */


/* ----- ANALYSIS functions ----- */
double * segment(double *sound, unsigned int frameNumber, unsigned int indexOffset, unsigned int frameStep, unsigned int hammingLength)
{
	double *seg = new double[hammingLength];
	unsigned int startIndex = indexOffset + frameNumber*frameStep;

	for (unsigned int n = 0; n < hammingLength; n++)
	{
		double win = 0.54 - 0.46 * cos(2 * pi*n / (hammingLength - 1));	// Make Hamming window on the fly
		seg[n] = win * sound[startIndex + n];
	}
	return seg;
}
double *findCoeffs(double *signal, int signalLength, int p)
{
	// Calculates the AR coefficients using the Levinson–Durbin algorithm
	double *acf = acorr(signal, signalLength, p + 1);
	double errorPower = acf[0];

	double *k = zeros(p + 1);
	double *a = zeros(p + 1);
	double *b = zeros(p + 1);

	b[0] = 1.0;

	for (int i = 1; i <= p; i++)
	{
		k[i] = acf[i];
		for (int j = 1; j < i; j++)
			k[i] -= b[j] * acf[i - j];
		k[i] /= errorPower;
		a[i] = k[i];

		for (int j = 1; j < i; j++)
			a[j] = b[j] - k[i] * b[i - j];

		errorPower *= (1 - pow(k[i], 2));
		for (int j = 1; j <= i; j++)
			b[j] = a[j];
	}
	double *c = new double[p];
	for (int i = 0; i < p; i++)
	{						// We don't want the first element, because it's always 1.
		c[i] = -b[i + 1];	// <- for practical reasons.
	}
	delete[] k;
	delete[] a;
	delete[] b;

	return c;
}
int findPeriod(double *signal, int signalLength, int rangeLow, int rangeHigh, double threshold)
{
	// ~Direct translation from MatLab
	double *acf = acorr(signal, signalLength, signalLength - 1);
	double max = 0;
	int L = 1;

	for (int n = 1; n < signalLength; n++)
	{
		if (acf[n] > acf[n - 1])
		{
			if (acf[n] > max)
			{
				max = acf[n];
				L = n;
			}
		}
	}
	if (L < rangeLow || L > rangeHigh)
		acf[L] = acf[0] * threshold / 2;	// For å sikre at ratio er under threshold

	if (acf[L] / acf[0] > threshold)
		return L;
	else
		return 0;
}
double findGain(double *signal, int signalLength)
{
	double power = 0;
	for (int n = 0; n < signalLength; n++)
	{
		power += pow(abs(signal[n]), 2);
	}
	return sqrt(power);
}
Parameters * analysis(double *sound, unsigned int soundLength, unsigned int p, double threshold, unsigned int numberOfFrames, unsigned int indexOffsetLPC, unsigned int frameStep, unsigned int winLengthLPC, unsigned int winLengthPA, int Fs)
{
	Parameters *paramArray = new Parameters[numberOfFrames];
	
	double prefilterB[2] = {1, -0.95};
	double prefilterA[1] = { -0.95 };
	double *soundPEF = filter(prefilterB, nullptr, sound, 2, 0, soundLength / 2);
	double *soundLPF = filter(nullptr, prefilterA, sound, 0, 1, soundLength / 2);
	/*The p and q of filter() seems weird because of the way filter() is made*/
	
	for (unsigned int f = 0; f < numberOfFrames; f++)
	{
		
		// LPC Analysis:
		double *segLPC = segment(sound, f, indexOffsetLPC, frameStep, winLengthLPC);
		paramArray[f].setCoeffs(p, findCoeffs(segLPC, winLengthLPC, p));

		double * inverseFilterB = addOne(paramArray[f].getCoeffs(), p);

		double * residual = filter(inverseFilterB, nullptr, segLPC, p + 1, 0, winLengthLPC);
		paramArray[f].setGain(findGain(residual, winLengthLPC));

		delete[] residual;
		delete[] inverseFilterB;
		delete[] segLPC;

		// Pitch analysis
		double *segPA = segment(sound, f, 0, frameStep, winLengthPA);	
		paramArray[f].setPeriod(findPeriod(segPA, winLengthPA, 32, 320, threshold));
		delete[] segPA;
	}
	
	delete[] soundPEF;
	delete[] soundLPF;

	return paramArray;
}
/* ------------------------------ */

/* ----- SYNTHESIS functions ----- */
double * generateNoise(unsigned int length)
{
	double * noise = new double[length];
	for (unsigned int i = 0; i < length; i++)
	{
		noise[i] = 2.0 * double(rand()) / RAND_MAX - 1;
	}
	return noise;
}
double * generateVoice(unsigned int period, unsigned int indexOffset, unsigned int length)
{
	double * pulsetrain = new double[length];
	for (int i = 0; i < int(length); i++)
	{
		if ((i - int(indexOffset)) % int(period) == 0)
			pulsetrain[i] = 1.0;
		else
			pulsetrain[i] = 0.0;
	}
	return pulsetrain;
}
double * synthesis(unsigned int p, unsigned int numberOfFrames, unsigned int frameStep, Parameters * params)
{
	unsigned int pulseIndexOffset = 0;
	double * excite = new double[frameStep];	// The AR excitation. For pure AR (unvoiced), this is white noise.
	double * newSegment = new double[frameStep];
	double * synthetic = new double[numberOfFrames * frameStep];


	for (unsigned int f = 0; f < numberOfFrames; f++)
	{
		unsigned int period = params[f].getPeriod();
		double gain = params[f].getGain();
		double * A = params[f].getCoeffs();	
		if (period == 0)
		{
			gain *= 0.1;	
			pulseIndexOffset = 0;
			excite = generateNoise(frameStep);
		}
		else
		{
			excite = generateVoice(period, pulseIndexOffset, frameStep);
			pulseIndexOffset = ((-int(frameStep) % (period - 1)) + pulseIndexOffset) % period;
		}
		newSegment = filter(nullptr, A, excite, 0, p, frameStep);
		for (unsigned int n = 0; n < frameStep; n++)
		{
			synthetic[f*frameStep + n] = gain * newSegment[n];
		}
	}
	delete[] newSegment;
	delete[] excite;
	return synthetic;
}

double * overlapAdd(double ** segments, unsigned int numberOfFrames, unsigned int segLength, unsigned int frameStep, unsigned int windowPadding)
{
	unsigned int sndLength = numberOfFrames * frameStep + 2 * windowPadding;
	double * outseq = zeros(sndLength);
	for (unsigned int f = 0; f < numberOfFrames; f++)
	{
		for (unsigned int n = 0; n < segLength; n++)
		{
			unsigned int ci = frameStep*f + n;	// current index
			outseq[ci] = outseq[ci] + segments[f][n];
		}
	}
	return outseq;
}
