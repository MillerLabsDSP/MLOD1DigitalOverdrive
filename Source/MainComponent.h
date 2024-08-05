/*
  ==============================================================================

    MainComponent.h
    Created: 11 Jul 2024 4:32:52pm
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SharedImages.h"
#include "SliderWidget.h"
#include "ButtonWidget.h"

//==============================================================================
/*
*/
class MainComponent  : public juce::Component
{
public:
    MainComponent(OverdrivePedalAudioProcessor&);
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void switchState();

private:
        
    int state = 1;
    
    OverdrivePedalAudioProcessor&           audioProcessor;
    SharedImages*                           m_pSharedImages;
    StompPedalButton                        stompPedalButton;
    LevelKnob                               levelKnob;
    DriveKnob                               driveKnob;
    ToneKnob                                toneKnob;
    
    std::vector<std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::vector<std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment>> buttonAttachments;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
