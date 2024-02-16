armoda
======

General repo for experiments involving AR-modeling of speech using JACK Audio Connection Kit.

## To do
1.  Rewrite AR-model from C++ to C. (see checklist below)

## Rewrite checklist
### Analysis functions:

    - [ ] `acorr()`         -   Compute the autocorrelation of the input
    - [ ] `findPeriod()`    -   Finding the second local maxima of acorr
    - [ ] `findCoeffs()`    -   Computing the LPC coeffs using Levinson-Durbin algorithm
    - [ ] `findGain()`      -   Simple, check speechanalysis.cpp

### Filtering functions:

    - [ ] `filterSelect()`  -   Selecting the 'active' filter.
    - [ ] `filter()`        -   Ideally use FFT Convolution to convolve input signal with filter.
          Input signal is an input to the program itself, for instance comming from a JACK synth.
