/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MainComponent.h"
#include <JuceHeader.h>

//==============================================================================
OverdrivePedalAudioProcessorEditor::OverdrivePedalAudioProcessorEditor (OverdrivePedalAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), mainComponent (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (276, 488);
    addAndMakeVisible(mainComponent);
    
}

OverdrivePedalAudioProcessorEditor::~OverdrivePedalAudioProcessorEditor()
{
}

//==============================================================================
void OverdrivePedalAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void OverdrivePedalAudioProcessorEditor::resized()
{
    mainComponent.setBounds(0, 0, getWidth(), getHeight());
}
