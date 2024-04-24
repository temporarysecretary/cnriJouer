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

    RegionMarker(int xPos, int full, int flag);

    ~RegionMarker();
    RegionMarker();
    void attach(RegionObserver *newObserver);
    void change();
    void mouseDown(const juce::MouseEvent &) override;

    int getLoopState();
    int getStartEndFlag();

    int getStartSample();
    void setStartSample(int newStart);
    int getEndSample();
    void setEndSample(int newEnd);
    static void setTotalSampleLength(int newLength);
    int getRegion();
    void setRegion(int newRegion);

    juce::Colour color;
    int region;

    static const int LOOP_DISABLED = 0;
    static const int LOOP_FORWARDS = 1;
    static const int LOOP_BACKWARDS = 2;
    static const int LOOP_PINGPONG = 3;

    static const int START = 0;
    static const int END = 1;
    static const int NEITHER = 2;

private:
    RegionObserver *observer;
    int loopState;
    int startEndFlag;

    static inline int totalSampleLength = 0;
    int startSample = 0;
    int endSample = totalSampleLength;
    void paint(juce::Graphics &g);

    void mouseEnter(const juce::MouseEvent &e);
};

class RegionObserver {
public:
    RegionObserver();
    void add(RegionMarker*);
    void update();
    void clear();
    void remove(RegionMarker*);
    static RegionMarker* getRegionMarker(int index);
    static int getSize();
private:
    static inline int size;
    static void sort();
    static inline std::vector<RegionMarker*> regionMarkers;
};

#endif //CNRIJOUER_REGIONMARKER_H
