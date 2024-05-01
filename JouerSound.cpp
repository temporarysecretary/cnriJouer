

#include "JouerSound.h"

// Does literally nothing for the most part

JouerSound::JouerSound(const juce::String &name, juce::AudioFormatReader &source, const juce::BigInteger &midiNotes,
                       int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds):
        juce::SamplerSound(name, source, midiNotes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds){

}

JouerSound::~JouerSound(){

}

bool JouerSound::appliesToRegion(int testRegion) {
    if(testRegion == region) return true;
    else return false;
}

void JouerSound::passEnvelope(juce::ADSR ad) {
    adsr = ad;
}

juce::ADSR JouerSound::returnADSR() {
    return juce::ADSR(adsr);
}
