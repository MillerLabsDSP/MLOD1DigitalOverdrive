/*
  ==============================================================================

    ClipperSection.cpp
    Created: 11 Jul 2024 3:59:45pm
    Author:  Zachary Miller

  ==============================================================================
*/

#include "ClipperSection.h"
#include <cmath>

void ClipperSection::prepareToPlay(double sampleRate, int bufferSize) {
    this->Fs = sampleRate;
    this->bufferSize = bufferSize;
}

float ClipperSection::returnSign(float number) {
    if (number > 0) {
        return 1;
    } else if (number < 0) {
        return -1;
    } else {
        return 0;
    }
}

float ClipperSection::processSample(float sample, int channel) {
    
    float y;
    
    sample *= drive;
    
    // Drive
    y = returnSign(sample) * (1.f - (1.f/(abs(30.f * sample) + 1.f)));
    
    // Level
    y *= level/5.f;
    
    // Limiter
    return y = abs(y) > 1 ? returnSign(y) : y;
    
}

void ClipperSection::processBuffer(float * samples, const int numSamples, const int channel) {
    for (int n = 0; n < numSamples ; n++) {
    
        inputHPF.setFreq(100.f);
        float x = inputHPF.processSample(samples[n], channel);
        
        samples[n] = processSample(x, channel);

    }

}

void ClipperSection::setDrive(float drive) {
    this->drive = drive;
}

void ClipperSection::setLevel(float level) {
    this->level = level;
}
