//
// Created by moonseekr on 4/15/2024.
//

#include "JouerVoice.h"

void debugFunction(float currSample, float counter, int startSample, int numSamples){
    std::cout << "Value: ";
    std::cout << currSample;
    std::cout << " ";
    std::cout << "counter: ";
    std::cout << counter;
    std::cout << " ";
    std::cout << "startSample: ";
    std::cout << startSample;
    std::cout << " ";
    std::cout << "Endpoint: ";
    std::cout << numSamples;
    std::cout << " ";
    std::cout << "\n";
}

void JouerVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if(auto* playingSound = static_cast<JouerSound*> (getCurrentlyPlayingSound().get())){

        auto help =  *playingSound->getAudioData();

        auto inL = help.getReadPointer(0);
        auto inR = help.getReadPointer(1);
        auto outL = outputBuffer.getWritePointer(0);
        auto outR = outputBuffer.getWritePointer(1);

        while(numSamples-- >= 0){
            auto adsrGain = adsr.getNextSample();
            auto L = inL[(int) where];
            auto R = inR[(int) where];

            if(mode1){
                int stagger = (int) where - ((int) where % 24)/2;
                L = inL[stagger];
                R = inR[stagger];
            }

            if(mode2){
                L = clip(L * 5);
                R = clip(R * 5);
            }

            outL[startSample] += L * level * adsrGain;
            outR[startSample] += R * level * adsrGain;

            startSample++;
            where+=pitchRatio;

            if(where > help.getNumSamples() || !adsr.isActive())
            {
                stopNote(0, false);
                break;
            }
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

    if(dynamic_cast<JouerSound*>(sound) != nullptr){
        std::cout<<"on";
        std::cout<<"\n";
        where = 0;
        pitchRatio = (float) pow(2,(midiNoteNumber-60)/12.0);
        level = velocity;

        mode1 = ModeButtons::buttons[0].getToggleState();
        mode2 = ModeButtons::buttons[1].getToggleState();

        adsr = dynamic_cast<JouerSound*>(sound)->returnADSR();
        adsr.noteOn();
    }

}

void JouerVoice::stopNote(float velocity, bool allowTailOff) {
    if(allowTailOff) {
        std::cout<<"off: release";
        std::cout<<"\n";
        adsr.noteOff();
    }
    else {
        std::cout<<"off: cut";
        std::cout<<"\n";
        adsr.reset();
        clearCurrentNote();
    };
}

bool JouerVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<JouerSound*>(sound) != nullptr;
}

void JouerVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void JouerVoice::pitchWheelMoved(int newPitchWheelValue) {

}

float JouerVoice::clip(float value) {
    if (value >= 1) return 0.125 + ((rand() % 100 - 50)/10000);
    else if (value <= -1) return -0.125 + ((rand() % 100 - 50)/10000);
    else return value + ((rand() % 100 - 50)/10000);
}
