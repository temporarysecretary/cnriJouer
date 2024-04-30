//
// Created by moonseekr on 2/29/2024.
//

#ifndef CNRIJOUER_MODEBUTTONS_H
#define CNRIJOUER_MODEBUTTONS_H
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class ModeButtons: public juce::Component{
public:
    ModeButtons(AudioPluginAudioProcessor& g);
    ~ModeButtons();

private:
    const static int BUTTON_COUNT = 5;
    static inline std::array<juce::ImageButton, BUTTON_COUNT> buttons;
//    std::array<juce::File, BUTTON_COUNT> imageFiles;
//    std::array<juce::Image,BUTTON_COUNT> images;

    void initializeButtons();

    // Overrides Component class
    void paint(juce::Graphics &g);
    void resized();

    //To access valuetree
    AudioPluginAudioProcessor& processorRef;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mode1Attch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mode2Attch;

};


#endif //CNRIJOUER_MODEBUTTONS_H

