/*
  ==============================================================================

    ButtonWidget.h
    Created: 11 Jul 2024 11:58:51pm
    Author:  Zachary Miller

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SharedImages.h"

class StompPedalButton : public juce::ImageButton {
public:
    
    StompPedalButton(SharedImages* i) : stompPedalButton(i->getStompPedalButton()) {
        
        setImages(false, true, true,
                  stompPedalButton, 1.f, juce::Colours::transparentWhite,
                  stompPedalButton, 0.9f, juce::Colours::transparentWhite,
                  stompPedalButton, 1.f, juce::Colours::transparentWhite);

    };
    
    ~StompPedalButton() {};
    

private:
    
    juce::Image& stompPedalButton; // up and down
            
};
