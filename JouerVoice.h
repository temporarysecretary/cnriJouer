//
// Created by moonseekr on 4/15/2024.
//

#ifndef CNRIJOUER_JOUERVOICE_H
#define CNRIJOUER_JOUERVOICE_H
#include <juce_audio_formats/juce_audio_formats.h>
#include "JouerSound.h"
#include "RegionMarker.h"

class JouerVoice: public juce::SynthesiserVoice {
public:
    JouerVoice() = default;
    ~JouerVoice() override = default;

    // Implementing functions from SynthesiserVoice
    void renderNextBlock (juce::AudioBuffer<float> &, int startSample, int numSamples) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    bool canPlaySound(juce::SynthesiserSound* ) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;

    static void updateModes(bool mode1, bool mode2);

    // My functions
    float clip(float value);

private:
    static inline bool mode1 = false;
    static inline bool mode2 = false;
    juce::ADSR adsr;

    static inline int activeRegion;

    float start = 0;
    float end = 0;
    float mover = 0;

    float pitchRatio = 1;
    float level = 0.15;
    double sampleRate = 44100;
};


#endif //CNRIJOUER_JOUERVOICE_H
