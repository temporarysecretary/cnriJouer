//
// Created by moonseekr on 4/15/2024.
//

#ifndef CNRIJOUER_JOUERSOUND_H
#define CNRIJOUER_JOUERSOUND_H
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "FileHolder.h"
#include "RegionMarker.h"

class JouerSound: public juce::SamplerSound {
public:
    JouerSound(const juce::String &name, juce::AudioFormatReader &source, const juce::BigInteger &midiNotes,
               int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds);
    ~JouerSound();
    void passEnvelope(juce::ADSR);
    void setRegion(int newRegion);
    bool appliesToRegion(int testRegion);
    int getRegionLoopProp();
    int getStartSample();
    juce::ADSR returnADSR();

private:
    juce::ADSR adsr;
    int region;
};


#endif //CNRIJOUER_JOUERSOUND_H
