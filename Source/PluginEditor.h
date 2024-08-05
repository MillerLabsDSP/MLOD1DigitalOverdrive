/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainComponent.h"

//==============================================================================
/**
*/
class OverdrivePedalAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    OverdrivePedalAudioProcessorEditor (OverdrivePedalAudioProcessor&);
    ~OverdrivePedalAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    OverdrivePedalAudioProcessor& audioProcessor;
    
    MainComponent           mainComponent;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdrivePedalAudioProcessorEditor)
};
