/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SharedImages.h"
#include "ClipperSection.h"
#include "ToneSection.h"

//==============================================================================
/**
*/
class OverdrivePedalAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    OverdrivePedalAudioProcessor();
    ~OverdrivePedalAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    
    SharedImages* getSharedImagesPtr() { return m_pSharedImagesPtr; };
    
    float drive = 1.f;
    float tone = 1.f;
    float level = 1.f;
    bool toggle = false;
    
    static const juce::StringRef DRIVE;
    static const juce::StringRef TONE;
    static const juce::StringRef LEVEL;
    static const juce::StringRef TOGGLEBUTTON;
    
    void driveChanged(float value);
    void toneChanged(float value);
    void levelChanged(float value);
    void pedalToggled(bool state);

private:
    
    int ParameterVersionHint = 1;
    
    juce::SharedResourcePointer<SharedImages>       m_pSharedImagesPtr;
    
    ClipperSection                                  clipperSection;
    ToneSection                                     toneSection;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdrivePedalAudioProcessor)
};
