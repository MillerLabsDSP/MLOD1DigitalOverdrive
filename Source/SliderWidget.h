/*
  ==============================================================================

    SliderWidget.h
    Created: 12 Jul 2024 12:03:39am
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SharedImages.h"

class BigKnob : public juce::Slider {
public:
    
    BigKnob(SharedImages* i) : knobImage(i->getBigKnobImage()) {
        
        setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        
    }
    
    ~BigKnob() {};
    
    void paint(juce::Graphics& g) override {
        
        /* Normalizing the slider value */
        const double fractRotation = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        
        int frameIndex = floor(fractRotation * (numFrame - 1)); // -1 to start at 0 like MATLAB
        
        const int startY = frameIndex * frameHeight; // indexes down strip for correct frame
        
        g.drawImage(knobImage, 0, 0, getWidth(), getHeight(), 0, startY, frameWidth, frameHeight);
    }
    
private:
    
    juce::Image& knobImage;
    
    int numFrame = 245;
    int frameHeight = 490;
    int frameWidth = 430;
    
    
};

class MediumKnob : public juce::Slider {
public:
    
    MediumKnob(SharedImages* i) : knobImage(i->getMediumKnobImage()) {
        
        setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        
    }
    
    ~MediumKnob() {};
    
    void paint(juce::Graphics& g) override {
        
        /* Normalizing the slider value */
        const double fractRotation = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        
        int frameIndex = floor(fractRotation * (numFrame - 1)); // -1 to start at 0 like MATLAB
        
        const int startY = frameIndex * frameHeight; // indexes down strip for correct frame
        
        g.drawImage(knobImage,0, 0, getWidth(), getHeight(), 0, startY, frameWidth, frameHeight);
    }
    
private:
    
    juce::Image& knobImage;
    
    int numFrame = 128;
    int frameHeight = 252;
    int frameWidth = 212;
    
    
};

class DriveKnob : public juce::Slider {
public:
    
    DriveKnob(SharedImages* i) : knobImage(i->getDriveLevelKnobImage()) {
        
        setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        
    }
    
    ~DriveKnob() {};
    
    void paint(juce::Graphics& g) override {
        
        /* Normalizing the slider value */
        const double fractRotation = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        
        int frameIndex = floor(fractRotation * (numFrame - 1)); // -1 to start at 0 like MATLAB
        
        const int startY = frameIndex * frameHeight; // indexes down strip for correct frame
        
        g.drawImage(knobImage, 0, 0, getWidth(), getHeight(), 0, startY, frameWidth, frameHeight);
    }
    
private:
    
    juce::Image& knobImage;
    
    int numFrame = 256;
    int frameHeight = 128;
    int frameWidth = 128;
    
    
};

class LevelKnob : public juce::Slider {
public:
    
    LevelKnob(SharedImages* i) : knobImage(i->getDriveLevelKnobImage()) {
        
        setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        
    }
    
    ~LevelKnob() {};
    
    void paint(juce::Graphics& g) override {
        
        /* Normalizing the slider value */
        const double fractRotation = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        
        int frameIndex = floor(fractRotation * (numFrame - 1)); // -1 to start at 0 like MATLAB
        
        const int startY = frameIndex * frameHeight; // indexes down strip for correct frame
        
        g.drawImage(knobImage, 0, 0, getWidth(), getHeight(), 0, startY, frameWidth, frameHeight);
    }
    
private:
    
    juce::Image& knobImage;
    
    int numFrame = 256;
    int frameHeight = 128;
    int frameWidth = 128;
    
    
};

class ToneKnob : public juce::Slider {
public:
    
    ToneKnob(SharedImages* i) : knobImage(i->getToneKnobImage()) {
        
        setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        
    }
    
    ~ToneKnob() {};
    
    void paint(juce::Graphics& g) override {
        
        /* Normalizing the slider value */
        const double fractRotation = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
        
        int frameIndex = floor(fractRotation * (numFrame - 1));
        
        const int startY = frameIndex * frameHeight; // indexes down strip for correct frame
        
        g.drawImage(knobImage, 0, 0, getWidth(), getHeight(), 0, startY, frameWidth, frameHeight);
    }
    
private:
    
    juce::Image& knobImage;
    
    int numFrame = 256;
    int frameHeight = 73;
    int frameWidth = 74;
    
    
};
