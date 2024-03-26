//
// Created by moonseekr on 2/29/2024.
//

#ifndef CNRIJOUER_MODEBUTTONS_H
#define CNRIJOUER_MODEBUTTONS_H
#include <juce_gui_basics/juce_gui_basics.h>

class ModeButtons: public juce::Component {

private:
    const static int BUTTON_COUNT = 5;
    std::array<juce::ImageButton, BUTTON_COUNT> buttons;

//    std::array<juce::File, BUTTON_COUNT> imageFiles;
//    std::array<juce::Image,BUTTON_COUNT> images;

    void initializeButtons();

    // Overrides Component class
    void paint(juce::Graphics &g);
    void resized();

public:
    ModeButtons();
    ~ModeButtons();
};


#endif //CNRIJOUER_MODEBUTTONS_H
