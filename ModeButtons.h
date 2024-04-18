//
// Created by moonseekr on 2/29/2024.
//

#ifndef CNRIJOUER_MODEBUTTONS_H
#define CNRIJOUER_MODEBUTTONS_H
#include <juce_gui_basics/juce_gui_basics.h>

class ModeButtons: public juce::Component, private juce::Button::Listener {

private:
    const static int BUTTON_COUNT = 5;

//    std::array<juce::File, BUTTON_COUNT> imageFiles;
//    std::array<juce::Image,BUTTON_COUNT> images;

    void initializeButtons();

    // Overrides Component class
    void paint(juce::Graphics &g);
    void resized();

    void buttonClicked(juce::Button *) override;

public:
    static inline std::array<juce::ImageButton, BUTTON_COUNT> buttons;
    ModeButtons();
    ~ModeButtons();
};


#endif //CNRIJOUER_MODEBUTTONS_H

