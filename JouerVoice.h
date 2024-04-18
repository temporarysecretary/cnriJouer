//
// Created by moonseekr on 4/15/2024.
//

#ifndef CNRIJOUER_JOUERVOICE_H
#define CNRIJOUER_JOUERVOICE_H
#include <juce_audio_formats/juce_audio_formats.h>
#include "JouerSound.h"
#include "ModeButtons.h"

class JouerVoice: public juce::SynthesiserVoice {
public:
    JouerVoice() = default;
    ~JouerVoice() override = default;
    void renderNextBlock (juce::AudioBuffer<float> &, int startSample, int numSamples) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void setADSRParams(juce::ADSR::Parameters);
    bool canPlaySound(juce::SynthesiserSound* ) override;
    void setSampleRate(int newSampleRate);

private:
    juce::ADSR adsr;
    float pitchRatio;
    float level;
    float tailoff;
    int sampleRate;

};


#endif //CNRIJOUER_JOUERVOICE_H
