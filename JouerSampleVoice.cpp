//
// Created by moonseekr on 4/1/2024.
//

#include "JouerSampleVoice.h"

JouerSampleVoice::JouerSampleVoice() {

}

JouerSampleVoice::~JouerSampleVoice(){

}

void JouerSampleVoice::updateModesState(std::array<bool, 5> modes){
    modesState = modes;
}

void JouerSampleVoice::renderNextBlock(juce::AudioBuffer<float> &, int startSample, int numSamples){

}

void JouerSampleVoice::destroy(juce::AudioBuffer<float> &){

}

void JouerSampleVoice::hold(juce::AudioBuffer<float> &){

}