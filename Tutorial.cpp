//
// Created by moonseekr on 4/29/2024.
//

#include "Tutorial.h"

Tutorial::Tutorial(){

    // make the button
    out.setButtonText("X");
    out.onClick = [this]{
        setVisible(false);
    };

    addAndMakeVisible(&out);
}

void Tutorial::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour(0x88FFFFFF));
    g.setFont(juce::Font("Century Gothic", 14, juce::Font::FontStyleFlags::plain));
    g.drawFittedText("jouer is a sampler meant for play \n"
                     "drag and drop a sample to get started\n"
                     "click on the waveform to create region markers\n"
                     "click region markers to loop forward (green), backwards(blue), or pingpong (red)\n"
                     "use the buttons in the bottom left to change the sound\n"
                     "use the sliders on the right side to change the dynamics\n",
                     this->getBounds(), juce::Justification::Flags::centred, 6);
}

void Tutorial::resized() {
    out.setBounds(10,10,10,10);
}