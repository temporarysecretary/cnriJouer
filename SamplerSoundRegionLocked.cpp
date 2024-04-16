//
// Created by moonseekr on 4/15/2024.
//

#include "SamplerSoundRegionLocked.h"

SamplerSoundRegionLocked::SamplerSoundRegionLocked(const juce::String &name, juce::AudioFormatReader &source, const juce::BigInteger &midiNotes,
                         int midiNoteForNormalPitch, double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds):
        juce::SamplerSound(name, source, midiNotes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds){

}

SamplerSoundRegionLocked::~SamplerSoundRegionLocked(){

}

void SamplerSoundRegionLocked::setRegion(int newRegion){
    region = newRegion;
}

bool SamplerSoundRegionLocked::appliesToRegion(int testRegion) {
    if(testRegion == region) return true;
    else return false;
}