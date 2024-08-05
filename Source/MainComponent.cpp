/*
  ==============================================================================

    MainComponent.cpp
    Created: 11 Jul 2024 4:32:52pm
    Author:  Zachary Miller

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent(OverdrivePedalAudioProcessor& p) :
audioProcessor          (p),
m_pSharedImages         (audioProcessor.getSharedImagesPtr()),
stompPedalButton        (m_pSharedImages),
levelKnob               (m_pSharedImages),
driveKnob               (m_pSharedImages),
toneKnob                (m_pSharedImages) {
    
    // Level knob
    addAndMakeVisible(levelKnob);
    levelKnob.onValueChange = [this](){
        audioProcessor.levelChanged(levelKnob.getValue());
    };
    
    // Drive knob
    addAndMakeVisible(driveKnob);
    driveKnob.onValueChange = [this](){
        audioProcessor.driveChanged(driveKnob.getValue());
    };

    // Tone knob
    addAndMakeVisible(toneKnob);
    toneKnob.onValueChange = [this](){
        audioProcessor.toneChanged(toneKnob.getValue());
    };
    
    // Stomp pedal button
    addAndMakeVisible(stompPedalButton);
    stompPedalButton.addMouseListener(this, false);
    stompPedalButton.onClick = [this](){
        switchState();
    };

    sliderAttachments.emplace_back (std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment> (p.getAPVTS(), "DRIVE", driveKnob));
    sliderAttachments.emplace_back (std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment> (p.getAPVTS(), "LEVEL", levelKnob));
    sliderAttachments.emplace_back (std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment> (p.getAPVTS(), "TONE", toneKnob));
    buttonAttachments.emplace_back (std::make_unique <juce::AudioProcessorValueTreeState::ButtonAttachment> (p.getAPVTS(), "TOGGLEBUTTON", stompPedalButton));
    
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (juce::Graphics& g)
{
    
    int width = getWidth();
    int height = getHeight();
    
    // Redraw on/off CHECK light
    if (audioProcessor.toggle == true) {
        g.drawImage(m_pSharedImages->getBackgroundImage_ON(), 0, 0, width, height, 0, 0, 1104, 1952);
    }
    if (audioProcessor.toggle == false) {
        g.drawImage(m_pSharedImages->getBackgroundImage_OFF(), 0, 0, width, height, 0, 0, 1104, 1952);
    }
}

void MainComponent::resized()
{
    levelKnob.setBounds                     (2, 10, 120, 120);
    driveKnob.setBounds                     (154, 12, 120, 120);
    toneKnob.setBounds                      (102, 76, 72, 72);
    stompPedalButton.setBounds              (0, 155, 276, 335);
}

void MainComponent::switchState() {
    if (audioProcessor.toggle == true) {
        audioProcessor.toggle = false;
    } else if (audioProcessor.toggle == false) {
        audioProcessor.toggle = true;
    }
}
