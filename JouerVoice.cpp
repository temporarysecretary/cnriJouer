//
// Created by moonseekr on 4/15/2024.
//

#include "JouerVoice.h"
void JouerVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if(auto* playingSound = static_cast<JouerSound*> (getCurrentlyPlayingSound().get())){
        auto data = *playingSound->getAudioData();
        auto lengthOfSound = data.getNumSamples();
        float counter = 0;
        auto currADSR = adsr.getNextSample();

        // If we're currently tailing off because the note has been stopped and tailoff requested
        if(tailoff > 0.0){
            while(--numSamples >= 0 && currADSR > 0.0 && (int) counter < lengthOfSound){
                for(int i = 0; i < outputBuffer.getNumChannels(); i++){
                    auto currSample = data.getSample(i, (int) counter);

                    if(ModeButtons::buttons[0].isDown()){
                        currSample *= 2;
                        if(currSample > 1){ currSample = 1; }
                        else if (currSample <= -1) {currSample = -1;}
                    }

                    outputBuffer.addSample(i, startSample,
                                           currSample * level);
                }

                counter += (1 * pitchRatio);
                startSample++;
            }
            adsr.reset();
            tailoff = 0;
        }

        else{
            while(numSamples-- >=0 && (int) counter < lengthOfSound){
                for(int i = 0; i < outputBuffer.getNumChannels(); i++){
                    auto currSample = data.getSample(i, (int) counter);

                    if(ModeButtons::buttons[0].isDown()){
                        currSample *= 2;
                        if(currSample > 1){ currSample = 1; }
                        else if (currSample <= -1) {currSample = -1;}
                    }

                    outputBuffer.addSample(i, startSample,
                                           currSample * level * currADSR);
                }
                counter += (1 * pitchRatio);
                startSample++;
            }
            stopNote(0, false);
        }
    }
}

void JouerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound,
                           int currentPitchWheelPosition) {

    // pitchRatio function adapted from https://newt.phys.unsw.edu.au/jw/notes.html
    // We use it here as a percentage to tell the sampler how fast to go
    // So midi note 81 (an octave above 69) will return a pitch ratio of 2, and so, over in renderNextBlock
    // We will be moving TWO samples per loop instead of just one- going twice as fast
    // Likewise, we will be moving at half a sample per loop at midi note 57
    // I could define a root note variable. I might do that later

    pitchRatio = (float) pow(2,(midiNoteNumber-69)/12);
    level = velocity * 0.15;
    tailoff = 0.0;
    adsr.noteOn();
}

void JouerVoice::stopNote(float velocity, bool allowTailOff) {
    if(allowTailOff) {
        adsr.noteOff();
        tailoff = 1.0;
    }
    else {
        tailoff = 0.0;
        adsr.reset();
    };
}

bool JouerVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<JouerSound*>(sound) != nullptr;
}

void JouerVoice::setADSRParams(juce::ADSR::Parameters params) {
    adsr.setParameters(params);
    adsr.reset();
}

void JouerVoice::setSampleRate(int newSampleRate) {
    sampleRate = newSampleRate;
    adsr.setSampleRate(sampleRate);
}
