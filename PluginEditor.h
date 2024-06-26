#pragma once

#include "PluginProcessor.h"
#include "DynamicsControlsComponent.h"
#include "WaveformWindow.h"
#include "FileHolder.h"
#include "ModeButtons.h"
#include "RegionOverlay.h"
#include "Tutorial.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &,
                                             juce::MidiKeyboardComponent keyboardComponent);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    WaveformWindow waveformWindow;
    ModeButtons modeButtons;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    DynamicsControlsComponent dynamicsControlsComponent;
    RegionOverlay regionOverlay;
    Tutorial tutorial;

    juce::TextButton save;
    juce::TextButton load;
    juce::TextButton showTutorial;

    // File dialogue box
    std::unique_ptr<juce::FileChooser> chooser;
    void loadFunc();
    void saveFunc();
    juce::XmlElement loadedPreset;

    juce::Label windowLabel;

    void timerCallback() override{
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }


    double width;
    double height;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
