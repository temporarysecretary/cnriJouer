//
// Created by moonseekr on 2/5/2024.
//

#ifndef CNRIJOUER_WAVEFORMWINDOW_H
#define CNRIJOUER_WAVEFORMWINDOW_H

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "PluginProcessor.h"

class WaveformWindow : public juce::Component, private juce::ChangeListener, public juce::FileDragAndDropTarget,
public juce::Slider::Listener{
public:
    WaveformWindow(AudioPluginAudioProcessor &p);
    ~WaveformWindow();
    juce::Rectangle<int> waveformWindowBounds;

    // For creating the waveform.
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnail thumbnail;
    float gainOfThumbnail;

    // Drawing & positioning functions
    void paint(juce::Graphics &g) override;
    void resized() override;
    void paintIfNoFileLoaded(juce::Graphics &g, const juce::Rectangle<int> bounds);
    void paintIfFileLoaded(juce::Graphics &g, const juce::Rectangle<int> bounds);

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    void thumbnailChanged();

    // Implemented functions from juce::FileDragAndDropTarget
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void fileDragEnter(const juce::StringArray &files, int x, int y) override;
    void fileDragExit(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;

    // Implemented functions from juce::Slider::Listener
    void sliderValueChanged (juce::Slider *slider) override;


    // Function for configuring formatManager
    void setAvailableAudioFormats();

    AudioPluginAudioProcessor& processorRef;
};

#endif //CNRIJOUER_WAVEFORMWINDOW_H
