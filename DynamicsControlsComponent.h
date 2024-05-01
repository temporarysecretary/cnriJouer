
// The aim of this class is to provide
// a SINGLE collection of controls that
// are necessary to properly control
// the dynamics of the synthesizer.

#ifndef CNRIJOUER_DYNAMICSCONTROLSCOMPONENT_H
#define CNRIJOUER_DYNAMICSCONTROLSCOMPONENT_H

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "PluginProcessor.h"

class DynamicsControlsComponent : public juce::Component{
public:
    DynamicsControlsComponent(AudioPluginAudioProcessor&);
    ~DynamicsControlsComponent();

private:
    static const int SLIDER_COUNT = 4;
    std::array<juce::Slider,SLIDER_COUNT> ADSRSliderArray;
    std::array<juce::Label,SLIDER_COUNT> ADSRLabelArray;

    AudioPluginAudioProcessor& processorRef;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttch;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttch;

    void paint(juce::Graphics &g) override;
    void resized() override;
};


#endif //CNRIJOUER_DYNAMICSCONTROLSCOMPONENT_H
