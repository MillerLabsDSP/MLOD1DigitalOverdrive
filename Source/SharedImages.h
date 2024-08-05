/*
  ==============================================================================

    SharedImages.h
    Created: 11 Jul 2024 4:38:20pm
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SharedImages {
public:
    
    SharedImages() { loadImages(); };
    
    void loadImages();
    
    juce::Image& getBackgroundImage_ON()        { return m_BackgroundImage_ON; };
    
    juce::Image& getBackgroundImage_OFF()       { return m_BackgroundImage_OFF; };
    
    juce::Image& getStompPedalButton()          { return m_StompPedalButton; };
        
    juce::Image& getMediumKnobImage()           { return m_MediumKnob; };
    
    juce::Image& getBigKnobImage()              { return m_BigKnob; };
    
    juce::Image& getDriveLevelKnobImage()       { return m_DriveLevelKnob; };
    
    juce::Image& getToneKnobImage()             { return m_ToneKnob; };
    
private:
    
    juce::Image m_BackgroundImage_ON;
    juce::Image m_BackgroundImage_OFF;
    juce::Image m_StompPedalButton;
    juce::Image m_MediumKnob;
    juce::Image m_BigKnob;
    juce::Image m_DriveLevelKnob;
    juce::Image m_ToneKnob;
    
};
