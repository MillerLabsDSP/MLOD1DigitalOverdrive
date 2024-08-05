/*
  ==============================================================================

    ClipperSection.h
    Created: 11 Jul 2024 3:59:45pm
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Biquad.h"

class ClipperSection {
public:
    
    void prepareToPlay(double sampleRate, int bufferSize);
    float returnSign(float number);
    float processSample(float sample, int channel);
    void processBuffer(float * samples, const int numSamples, const int channel);
    
    void setDrive(float drive);
    void setLevel(float level);

private:
    
    Biquad inputHPF {Biquad::FilterType::HPF, Biquad::FilterQuality::Standard, 0.7071};

    float Fs = -1.f;
    float bufferSize = -1.f;
    float drive = 1.f;
    float level = 1.f;
    float dBLevel = 0.f;
};
