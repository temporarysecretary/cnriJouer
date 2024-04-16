//
// Created by moonseekr on 4/1/2024.
//

#ifndef CNRIJOUER_REGIONMARKER_H
#define CNRIJOUER_REGIONMARKER_H
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "FileHolder.h"

// Forward declaration of RegionObserver so that we can reference it at some other point
class RegionObserver;

class RegionMarker: public juce::Component {
public:
    RegionMarker(int xPos, int full);
    ~RegionMarker();
    RegionMarker();
    void attach(RegionObserver *newObserver);
    void change();
    void mouseDown(const juce::MouseEvent &) override;
    int getState() const;

    int startSample;
    int endSample;
    juce::Colour color;

    const int LOOP_DISABLED = 0;
    const int LOOP_FORWARDS = 1;
    const int LOOP_BACKWARDS = 2;
    const int LOOP_PINGPONG = 3;

private:
    RegionObserver *observer;
    int state;

    void paint(juce::Graphics &g);

    void mouseEnter(const juce::MouseEvent &e);
};


#endif //CNRIJOUER_REGIONMARKER_H
