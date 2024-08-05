/*
  ==============================================================================

    ToneSection.cpp
    Created: 11 Jul 2024 3:59:22pm
    Author:  Zachary Miller

  ==============================================================================
*/

#include "ToneSection.h"

void ToneSection::prepareToPlay(double sampleRate, int bufferSize) {
    this->Fs = sampleRate;
    this->bufferSize = bufferSize;
}

float ToneSection::processSample(float x, int channel) {
        
    // LSHELF
    LSHELF.setFs(150.f);
    LSHELF.setQ(0.7071);
    LSHELF.setAmpdB(-4.5f);
    float LSHELFout = LSHELF.processSample(x, channel);
    
    // PEAK
    PEAK.setFreq(3500.f);
    PEAK.setQ(0.7071);
    PEAKdBscl = -0.3f * tone; // 0-10 knob --> 0dB - 3dB reduction
    PEAK.setAmpdB(PEAKdBscl);
    float PEAKout = PEAK.processSample(LSHELFout, channel);
    
    // LPF
    LPFw0scl = 15000.f - 500 * tone ; // 0-10 knob --> 15kHz - 10kHz
    LPF.setFreq(LPFw0scl);
    LPF.setQ(0.2f);
    float LPFout = LPF.processSample(PEAKout, channel);
    
    float out = LPFout;

    return out;
}

void ToneSection::processBuffer(float * samples, const int numSamples, const int channel) {
    for (int n = 0; n < numSamples ; n++) {
    
        float x = samples[n];
        
        samples[n] = processSample(x, channel);

    }

}

void ToneSection::setTone(float tone) {
    this->tone = tone;
}
