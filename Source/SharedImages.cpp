/*
  ==============================================================================

    SharedImages.cpp
    Created: 11 Jul 2024 4:38:20pm
    Author:  Zachary Miller

  ==============================================================================
*/

#include "SharedImages.h"

void SharedImages::loadImages() {
    
    m_BackgroundImage_ON = juce::ImageCache::getFromMemory(BinaryData::MLOD1_base7ON_png, BinaryData::MLOD1_base7ON_pngSize);
    
    m_BackgroundImage_OFF = juce::ImageCache::getFromMemory(BinaryData::MLOD1_base7OFF_png, BinaryData::MLOD1_base7OFF_pngSize);
    
    m_StompPedalButton = juce::ImageCache::getFromMemory(BinaryData::MLOD1_pedalbutton1_png, BinaryData::MLOD1_pedalbutton1_pngSize);
    
    m_MediumKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_middle_png, BinaryData::Knob_middle_pngSize);
    
    m_BigKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_big_png, BinaryData::Knob_big_pngSize);
    
    m_DriveLevelKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_drivelevel3_png, BinaryData::Knob_drivelevel3_pngSize);
    
    m_ToneKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_tone2_png, BinaryData::Knob_tone2_pngSize);

}
