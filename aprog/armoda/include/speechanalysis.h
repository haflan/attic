#ifndef SPEECHANALYSIS_H
#define SPEECHANALYSIS_H

enum PadPosition{ PAD_FRONT, PAD_BACK };

class Parameters {
	// TODO: Rewrite this into a struct 
private:
	unsigned int order;			// The LPC filter order
	double * coeffs;			// The LPC coefficients
	unsigned int period;		// Segment period
	double gain;				// Segment gain
public:
	Parameters();
	~Parameters();
	void setAll(unsigned int order, double * coeffs, unsigned int period, double gain);
	void setCoeffs(unsigned int order, double *coeffs);
	void setPeriod(unsigned int period);
	void setGain(double gain);
	double getOrder(){ return this->order; }
	double * getCoeffs(){ return this->coeffs; }
	unsigned int getPeriod(){ return this->period; }
	double getGain(){ return this->gain; }
};

void deleteM(double ** multiarray, unsigned int rows);

double *acorr(double *signal, int signalLength, int maxLag);
double *zeros(int length);
double *zeroPadding(double *signal, int signalLength, int numberOfZeros, PadPosition position, bool override);
double *filter(double *B, double *A, double *x, int q, int p, int signalLength);
double *findCoeffs(double *signal, int signalLength, int p);
double findGain(double *signal, int signalLength);
int findPeriod(double *signal, int signalLength, int rangeLow, int rangeHigh, double threshold);
double * generateNoise(unsigned int N);
double * generateVoice(unsigned int period, unsigned int indexOffset, unsigned int length);
double * overlapAdd(double ** segments, unsigned int numberOfFrames, unsigned int segLength, unsigned int frameStep, unsigned int windowPadding);
double * addOne(double * coeffs, unsigned int p);

double maxabs(double *signal, int signalLength);
void normalize(double *signal, int signalLength);

double *segment(double *sound, unsigned int frameNumber, unsigned int indexOffset, unsigned int frameStep, unsigned int hammingLength);

Parameters * analysis(double * sound, unsigned int soundLength, unsigned int p, double threshold, unsigned int numberOfFrames, unsigned int indexOffsetLPC, unsigned int frameStep, unsigned int winLengthLPC, unsigned int winLengthPA, int Fs);

double * synthesis(unsigned int p, unsigned int numberOfFrames, unsigned int frameStep, Parameters * params);

Parameters * relpAnalysis(double *sound, unsigned int soundLength, unsigned int p, unsigned int numberOfFrames, unsigned int indexOffsetLPC, unsigned int frameStep, unsigned int winLengthLPC, double ** dsLFresidualSegments, unsigned int D);
double * relpSynthesis(double ** residualSegs, Parameters * params, unsigned int p, unsigned int D, unsigned int numberOfFrames, unsigned int segLength, unsigned int windowPadding, unsigned int frameStep, bool HFRegen);

#endif
