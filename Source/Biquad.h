//
//  Biquad.h
//
//  Copyright © 2019 Eric Tarr. All rights reserved.
//  Code may not be distributed without permission
//

/*
    This class implements the RBJ Audio EQ Cookbook Formulas

        http://shepazu.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
 
    There are options for all of the standard filter types: LPF, HPF, etc. These
    filters are great as the building blocks for more complicated (i.e. higher
    order) filters because the RBJ filters are stable and work will when
    parameters are changed.
 
    Note: A Butterworth filter (flat passband) can be created by setting the RBJ
    filter's Q=sqrt(2)/2
*/

#pragma once

class Biquad
{
public:
    enum FilterType
    {
        LPF,
        HPF,
        BPF,
        NOTCH,
        LSHELF,
        HSHELF,
        PEAK,
        APF
    };
    
    enum FilterQuality
    {
        Standard,
        HiFi
    };
    
    Biquad() {};
    Biquad(FilterType filterType, FilterQuality filterQuality, float Q) : filterType(filterType), filterQuality(filterQuality), Q(Q) {};
    
    float processSample(float x, int channel);

    void setFs(float Fs);

    void setFreq(double freq);
    double getCutoffFrequency() {return freq;}
    
    void setQ(double Q);

    void setAmpdB(double ampdB);

    void setFilterType (FilterType filterTypeParam)
    {
        this->filterType = filterTypeParam;
    }
    
    void setFilterQuality (FilterQuality filterQualityParam)
    {
        this->filterQuality = filterQualityParam;
        updateCoefficients();
    }

private:
    
    FilterType filterType = LPF;
    FilterQuality filterQuality = Standard;

    float Fs = 48000.0; // Sampling Rate

    // Variables for User to Modify Filter
    double freq = 20.0; // frequency in Hz
    double Q = 0.7071; // Q => [0.1 - 10]
    double ampdB = 0.0; // Amplitude on dB scale
    
    // Intermediate variables (all filters)
    double w0 = 0.f;
    double alpha = 0.f;
    double A = 0.f;

    // Variables for Biquad Implementation
    // 2 channels - L,R : Up to 2nd Order
    float x1[2] = { 0.0 }; // 1 sample of delay feedforward
    float x2[2] = { 0.0 }; // 2 samples of delay feedforward
    float y1[2] = { 0.0 }; // 1 sample of delay feedback
    float y2[2] = { 0.0 }; // 2 samples of delay feedback
    
    // Special case variables (Vicanek shelves)
    double shlvG, invG, sqrtGain = 0.0;
    
    double hny, fc4, f1, f14, h1, f2, f24, h2 = 0.0;
    
    double d1, c11, c12, d2, c21, c22, aa1, bb1, aa2, bb2, alpha1, v, w, V, W = 0.0;
    
    double inva0, sinrad2, q, G2, r0, r1, phi, phi0, phi1, phi2, R1, R2, G4 = 0.0;
    
    // Intermediate variables (Vicanek notch)
    double db1, scale = 0.0;
    
    // Vicanek biquad coefficients
    double A0, A1, A2, B0, B1, B2 = 0.0;

    // General biquad filter coefficients
    double b0, a0 = 1.0; // initialized to pass signal
    double b1, b2, a1, a2 = 0.0;

    void updateCoefficients();
    
};
