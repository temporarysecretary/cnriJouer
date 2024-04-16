//
// Created by moonseekr on 4/15/2024.
//

#ifndef CNRIJOUER_SAMPLERSOUNDREGIONLOCKED_H
#define CNRIJOUER_SAMPLERSOUNDREGIONLOCKED_H
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>

class SamplerSoundRegionLocked: public juce::SamplerSound {
public:
    SamplerSoundRegionLocked(const juce::String &name, juce::AudioFormatReader &source, const juce::BigInteger &midiNotes,
                             int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds);
    ~SamplerSoundRegionLocked();
    void setRegion(int newRegion);
    bool appliesToRegion(int testRegion);
private:
    int region;
};


#endif //CNRIJOUER_SAMPLERSOUNDREGIONLOCKED_H
