
#include "ModeButtons.h"
#include "BinaryData.h"

ModeButtons::ModeButtons(AudioPluginAudioProcessor& g):processorRef(g){
    // Initializes properties
    int width = (60) * BUTTON_COUNT;
    juce::Rectangle<int> modeButtonsBounds = juce::Rectangle<int>(0,0, width,60);
    setBounds(modeButtonsBounds);

    // Makes each button visible and sets their behavior to toggle
    for(auto &s:buttons){
        addAndMakeVisible(&s);
        s.setToggleable(true);
        s.setClickingTogglesState(true);
    }

    // Attachment to value tree
    mode1Attch = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.getApvts(), "MODE1", buttons[0]);
    mode2Attch = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.getApvts(), "MODE2", buttons[1]);

    initializeButtons();

};

ModeButtons::~ModeButtons()= default;

void ModeButtons::paint(juce::Graphics &g){
    g.fillAll(juce::Colours::whitesmoke);
}

void ModeButtons::resized(){
    // Dynamically splits the buttons area and separates each button.
    auto holdingArea = getLocalBounds();
    const auto widthOfSlider = static_cast<int>(holdingArea.getWidth()/BUTTON_COUNT);
    for(auto &s:buttons){
        s.setBounds(holdingArea.removeFromLeft(widthOfSlider).reduced(5));

    }
}

// Initializes button images from binary data in icons folder
void ModeButtons::initializeButtons(){
    std::array<const char*,BUTTON_COUNT> imageData = {
            BinaryData::_0_png, BinaryData::_1_png, BinaryData::_2_png, BinaryData::_3_png, BinaryData::_4_png
    };

    std::array<const int,BUTTON_COUNT> imageSizeData = {
            BinaryData::_0_pngSize, BinaryData::_1_pngSize, BinaryData::_2_pngSize, BinaryData::_3_pngSize, BinaryData::_4_pngSize
    };

    for(int i = 0; i < BUTTON_COUNT; i++){
        buttons[i].setImages(false, true, true,
                             juce::ImageCache::getFromMemory(imageData[i],imageSizeData[i]),
                             0.5f, juce::Colours::transparentWhite,
                             juce::Image(), 1.0f, juce::Colours::transparentWhite,
                             juce::Image(), 1.0f, juce::Colours::transparentBlack, 0);
        std::cout << "Button visible?" + std::to_string(buttons[i].isVisible()) + "\n";
    }
}