#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FileHolder.h"

//==============================================================================
// Constructor
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p,
                                                                 juce::MidiKeyboardComponent keyboardComponent)
    : AudioProcessorEditor (&p), processorRef (p), keyboardComponent(keyboardState, juce::KeyboardComponentBase::Orientation::horizontalKeyboard),
      waveformWindow(p), dynamicsControlsComponent(p)
    {
    width = 800;
    height = 500;
    juce::ignoreUnused (processorRef);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (width, height);
    std::cout << "constructing AE\n";

    addAndMakeVisible(&windowLabel);
    windowLabel.setFont(juce::Font("Century Gothic", 64, juce::Font::FontStyleFlags::plain));
    windowLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    windowLabel.setText("watch revue starlight bitch", juce::dontSendNotification);
    windowLabel.setJustificationType(juce::Justification::topLeft);

    addAndMakeVisible(&dynamicsControlsComponent);
    std::cout << "ADSR added\n";

    addAndMakeVisible(&waveformWindow);
    std::cout << "WW added\n";

    addAndMakeVisible(&modeButtons);
    std::cout << "Mode buttons added\n";

    addAndMakeVisible(&FileHolder::fileLabel);
    FileHolder::fileLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    FileHolder::fileLabel.setFont(juce::Font("Century Gothic", 24, juce::Font::FontStyleFlags::plain));
    std::cout << "file label added\n";

    addAndMakeVisible(&keyboardComponent);
    startTimer(400);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================



void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::white);
    g.setColour(juce::Colours::black);

}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    width = getWidth();
    height = getHeight();

    dynamicsControlsComponent.setBounds(583, 124,dynamicsControlsComponent.getWidth(), dynamicsControlsComponent.getHeight());

    waveformWindow.setBounds(44,140,waveformWindow.getWidth(),waveformWindow.getHeight());
    waveformWindow.toFront(false);

    windowLabel.setJustificationType(juce::Justification::topLeft);
    windowLabel.setBounds(44,44,getParentWidth(),getParentHeight());

    modeButtons.setBounds(43,400,modeButtons.getWidth(),modeButtons.getHeight());

    FileHolder::fileLabel.attachToComponent(&waveformWindow,false);
}
