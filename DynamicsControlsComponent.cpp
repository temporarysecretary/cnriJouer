//
// Created by moonseekr on 2/3/2024.
// This is the implementation of the DynamicsControlsComponent.

#include <juce_gui_basics/juce_gui_basics.h>
#include "DynamicsControlsComponent.h"
#include "PluginProcessor.h"
#include "FileHolder.h"
#include <cstdio>

juce::Rectangle<int> windowBounds(0,0,180,240);
juce::String ADSRLabelText[] = {"a","d","s","r"};

DynamicsControlsComponent::DynamicsControlsComponent(AudioPluginAudioProcessor& p): processorRef(p)
{
    setBounds(windowBounds);

    for (auto& s : ADSRSliderArray){
        // This loop creates sliders of the same style, with the same range
        // and the same bounds as established by the Rectangle above. This works
        // because they are all the same shape (as of 2/15/24) and have the same
        // vibe so to speak. It iterates over members of the ADSRSliderArray, of
        // which there are four.
        std::cout << "slider created!";
        s.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
        s.setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(0xff0099ff));
        s.addListener(this);
        addAndMakeVisible(&s);
    }

    // Initializes slider labels. It makes things neater to use arrays and looping through like this.
    for(int i = 0; i < SLIDER_COUNT; i++){
        addAndMakeVisible(&ADSRLabelArray[i]);
        ADSRLabelArray[i].setText(ADSRLabelText[i],juce::sendNotification);
        ADSRLabelArray[i].attachToComponent(&ADSRSliderArray[i],false);\
        ADSRLabelArray[i].setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        ADSRLabelArray[i].setFont(juce::Font("Century Gothic", 16, juce::Font::FontStyleFlags::plain));
        ADSRLabelArray[i].setJustificationType(juce::Justification::centred);
        if(i == 2){
            ADSRSliderArray[i].setRange(0.0,1.0,0.01);
            ADSRSliderArray[i].setValue(0.8, juce::dontSendNotification);
        }
        else{
            ADSRSliderArray[i].setRange(0.01,2,0.01);
            ADSRSliderArray[i].setValue(0.01, juce::dontSendNotification);
        }
    }
}

void DynamicsControlsComponent::paint(juce::Graphics &g) {

    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::black);
}

void DynamicsControlsComponent::resized(){

    // This loop is supposed to move the Sliders apart...
    // and it does!
    auto holdingArea = getLocalBounds();
    holdingArea.reduce(0,20);
    const auto widthOfSlider = static_cast<int>(holdingArea.getWidth()/ADSRSliderArray.size());
    for(auto &s:ADSRSliderArray){
        s.setBounds(holdingArea.removeFromLeft(widthOfSlider).reduced(10));
    }
}

void DynamicsControlsComponent::sliderValueChanged(juce::Slider *slider) {
    // Sends message to the audio processor with the new ADSR values
    // But only if FileHolder contains a valid sample i.e. it's not a nullptr...
    // Otherwise, this shit WILL explode

    if(FileHolder::doesSampleExist()){
        std::cout<<"Value tweaked";

        processorRef.setADSREnvelope(
                ADSRSliderArray[0].getValue(),
                ADSRSliderArray[1].getValue(),
                ADSRSliderArray[2].getValue(),
                ADSRSliderArray[3].getValue()
        );
    }
    }

DynamicsControlsComponent::~DynamicsControlsComponent() {

}
