//
// Created by moonseekr on 3/28/2024.
//

#ifndef CNRIJOUER_REGIONOVERLAY_H
#define CNRIJOUER_REGIONOVERLAY_H
#include <juce_gui_basics/juce_gui_basics.h>
#include "WaveformWindow.h"
#include "FileHolder.h"


class RegionOverlay: public juce::Component {
public:
    RegionOverlay();
    ~RegionOverlay();
private:
    juce::Rectangle<int> regionOverlayBounds;
    juce::Point<int> mousePosition;
    void resized() override;
    void paint(juce::Graphics &g) override;
    void RegionOverlay::paintOnMouseClick(juce::Graphics &g);
    void RegionOverlay::paintOnMouseMove(juce::Graphics &g);
    void RegionOverlay::mouseMove(const juce::MouseEvent &event) override;
    void RegionOverlay::mouseExit(const juce::MouseEvent &event);
};


#endif //CNRIJOUER_REGIONOVERLAY_H
