#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "FileHolder.h"
#include "JouerSound.h"
#include "JouerVoice.h"
#include "RegionMarker.h"

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor, juce::ValueTree::Listener
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadFile(juce::String path);

    // Save XML
    void saveXML();
    // Load XML
    void loadXML();

    void setADSREnvelope();
    void updateModes();
    juce::AudioProcessorValueTreeState& getApvts();
    juce::AudioFormatReader* mFormatReader{nullptr};

private:
    //==============================================================================
    const int numVoices = 8;
    juce::AudioFormatManager mFormatManager;


    // I used some community resources to learn how to handle the save state.
    // https://youtu.be/HrRghlZHJvE
    // After that, I extended that to boolean parameters for saving the mode states,
    // and then used the XMLElement class to add the file path.
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout initParams();
    void valueTreePropertyChanged(juce::ValueTree &, const juce::Identifier &) override;

    // File dialogue box
    std::unique_ptr<juce::FileChooser> chooser;

    std::atomic<bool> shouldADSRUpdate = false;

    juce::AudioParameterFloat* attack;
    juce::AudioParameterFloat* decay;
    juce::AudioParameterFloat* sustain;
    juce::AudioParameterFloat* release;

    juce::AudioParameterBool* mode1;
    juce::AudioParameterBool* mode2;

    juce::ADSR adsrEnvelope;
    juce::MidiKeyboardState keyboardState;
    juce::Synthesiser synth;
    JouerSound* loadedSample{nullptr};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
