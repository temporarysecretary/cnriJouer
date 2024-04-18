#pragma once

#include <vector>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "RegionMarker.h"

class RegionObserver {
public:
    RegionObserver();
    void add(RegionMarker*);
    void update();
    static RegionMarker* getRegionMarker(int index);
private:
    void sort();
    static inline std::vector<RegionMarker*> regionMarkers;
};