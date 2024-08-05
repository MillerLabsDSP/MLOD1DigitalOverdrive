/*
  ==============================================================================

    ToneSection.h
    Created: 11 Jul 2024 3:59:22pm
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include "Biquad.h"

class ToneSection {
public:
    
    void prepareToPlay(double sampleRate, int bufferSize);
    float processSample(float sample, int channel);
    void processBuffer(float * samples, const int numSamples, const int channel);
    
    void setTone(float tone);
    
private:
    
    Biquad LSHELF {Biquad::FilterType::LSHELF, Biquad::FilterQuality::Standard, 0.7071};
    Biquad LPF {Biquad::FilterType::LPF, Biquad::FilterQuality::Standard, 0.7071};
    Biquad PEAK {Biquad::FilterType::PEAK, Biquad::FilterQuality::Standard, 0.7071};
    
    float tone = -1.f;
    
    float HPFw0scl = 0.f;
    float LPFw0scl = 0.f;
    float PEAKdBscl = 0.f;
    
    float Fs = -1.f;
    float bufferSize = -1.f;
        
};
