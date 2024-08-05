/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

const juce::StringRef OverdrivePedalAudioProcessor::DRIVE = "DRIVE";
const juce::StringRef OverdrivePedalAudioProcessor::TONE = "TONE";
const juce::StringRef OverdrivePedalAudioProcessor::LEVEL = "LEVEL";
const juce::StringRef OverdrivePedalAudioProcessor::TOGGLEBUTTON = "TOGGLEBUTTON";

//==============================================================================
OverdrivePedalAudioProcessor::OverdrivePedalAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
apvts(*this, nullptr, "Params", createParams()) {
}

OverdrivePedalAudioProcessor::~OverdrivePedalAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout OverdrivePedalAudioProcessor::createParams() {
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"DRIVE", ParameterVersionHint}, "Drive", juce::NormalisableRange<float>(0.2f, 5.2f, 0.001f), 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"TONE", ParameterVersionHint}, "Tone", juce::NormalisableRange<float>(0.f, 10.f, 0.001f), 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LEVEL", ParameterVersionHint}, "Level", juce::NormalisableRange<float>(0.f, 10.f, 0.001f), 5.f));
    params.push_back(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"TOGGLEBUTTON", ParameterVersionHint}, "Toggle Button", false));

    return { params.begin(), params.end() };
    
}


//==============================================================================
const juce::String OverdrivePedalAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OverdrivePedalAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OverdrivePedalAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OverdrivePedalAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OverdrivePedalAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OverdrivePedalAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OverdrivePedalAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OverdrivePedalAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OverdrivePedalAudioProcessor::getProgramName (int index)
{
    return {};
}

void OverdrivePedalAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OverdrivePedalAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    clipperSection.prepareToPlay(sampleRate, samplesPerBlock);
    toneSection.prepareToPlay(sampleRate, samplesPerBlock);
}

void OverdrivePedalAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OverdrivePedalAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OverdrivePedalAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    clipperSection.setLevel(level);
    clipperSection.setDrive(drive);
    toneSection.setTone(tone);

    float numSamples = buffer.getNumSamples();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        if (toggle == false) {
            break;
        }
        
        clipperSection.processBuffer(channelData, numSamples, channel);
        toneSection.processBuffer(channelData, numSamples, channel);
        
    }
}

//==============================================================================
bool OverdrivePedalAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OverdrivePedalAudioProcessor::createEditor()
{
    return new OverdrivePedalAudioProcessorEditor (*this);
}

//==============================================================================
void OverdrivePedalAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto currentState = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (currentState.createXml());
    copyXmlToBinary(*xml, destData);
}

void OverdrivePedalAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    getXmlFromBinary(data, sizeInBytes);
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    juce::ValueTree newTree = juce::ValueTree::fromXml(*xml);
    apvts.replaceState(newTree);
}

void OverdrivePedalAudioProcessor::driveChanged(float value) {
    drive = value;
}

void OverdrivePedalAudioProcessor::toneChanged(float value) {
    tone = value;
}

void OverdrivePedalAudioProcessor::levelChanged(float value) {
    level = value;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OverdrivePedalAudioProcessor();
}
