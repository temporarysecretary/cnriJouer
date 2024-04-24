//
// Created by moonseekr on 3/28/2024.
//

#ifndef CNRIJOUER_REGIONOVERLAY_H
#define CNRIJOUER_REGIONOVERLAY_H
#include <juce_gui_basics/juce_gui_basics.h>
#include "FileHolder.h"
#include "RegionMarker.h"


class RegionOverlay: public juce::Component {
public:
    RegionOverlay();
    ~RegionOverlay();

    void initFirstRegionMarker();

private:
    RegionObserver *regionObserver;
    juce::Rectangle<int> regionOverlayBounds;
    juce::Point<int> mousePosition;

    void resized() override;
    void paint(juce::Graphics &g) override;
    void paintOnMouseMove(juce::Graphics &g);

    void mouseMove(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;
    void mouseDown(const juce::MouseEvent &event) override;
};


#endif //CNRIJOUER_REGIONOVERLAY_H
