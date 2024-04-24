//
// Created by moonseekr on 2/22/2024.
// This class holds a global variable, activeSample,
// as well as relevant information pertaining to it.

#ifndef CNRIJOUER_FILEHOLDER_H
#define CNRIJOUER_FILEHOLDER_H

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_gui_basics/juce_gui_basics.h"

class FileHolder {
private:
    static juce::File activeSample;
    forcedinline static juce::AudioBuffer<float> activeSampleBuffer;
public:
    forcedinline static juce::String path;
    static juce::Label fileLabel;
    static void setActiveSample(const juce::File& newSample);
    static juce::File getActiveSample();
    static bool doesSampleExist();

    static void setPath(const juce::String &string);
};


#endif //CNRIJOUER_FILEHOLDER_H
