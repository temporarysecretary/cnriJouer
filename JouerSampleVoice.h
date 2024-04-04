//
// Created by moonseekr on 4/1/2024.
//

#ifndef CNRIJOUER_JOUERAUDIOSOURCE_H
#define CNRIJOUER_JOUERAUDIOSOURCE_H
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>

class JouerSampleVoice: public juce::SamplerVoice {
public:
    JouerSampleVoice();
    ~JouerSampleVoice();
    static void updateModesState(std::array<bool, 5> modes);

private:
    void renderNextBlock(juce::AudioBuffer<float> &, int startSample, int numSamples) override;
    void destroy(juce::AudioBuffer<float> &);
    void hold(juce::AudioBuffer<float> &);

    static std::array<bool, 5> modesState;
};


#endif //CNRIJOUER_JOUERAUDIOSOURCE_H
