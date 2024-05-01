//
// Created by moonseekr on 4/29/2024.
//

#ifndef CNRIJOUER_TUTORIAL_H
#define CNRIJOUER_TUTORIAL_H
#include <juce_gui_basics/juce_gui_basics.h>

class Tutorial: public juce::Component {
public:
    Tutorial();
private:
    juce::TextButton out;
    void paint(juce::Graphics &g) override;
    void resized() override;
};


#endif //CNRIJOUER_TUTORIAL_H
