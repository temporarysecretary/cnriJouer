#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FileHolder.h"

//==============================================================================
// Constructor
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p,
                                                                 juce::MidiKeyboardComponent keyboardComponent)
    : AudioProcessorEditor (&p), processorRef (p), keyboardComponent(keyboardState, juce::KeyboardComponentBase::Orientation::horizontalKeyboard),
      waveformWindow(p), dynamicsControlsComponent(p), modeButtons(p)
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
    windowLabel.setText("jouer demo :)", juce::dontSendNotification);
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

    addAndMakeVisible(&save);
    addAndMakeVisible(&load);

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

    save.setBounds(0,0,40,20);
    save.setButtonText("save");
    save.setTopRightPosition(780,470);

    // I originally wrote this in the PluginProcessor file but realized it needed the scope of PluginEditor.
    save.onClick = [this]{
        std::cout<<"yoohoo\n";

        chooser = std::make_unique<juce::FileChooser>(
                "Choose where to save this preset.", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                "*.jouerxml"
        );

        auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
                              {
                                  auto xmlElement = this->processorRef.getApvts().state.createXml();
                                  auto path = juce::XmlElement::createTextElement(FileHolder::path);
                                  path->setTagName("PATH");
                                  xmlElement->addChildElement(path);

                                  auto meta = new juce::XmlElement("SAMPLE_META");
                                  meta->setAttribute("length", RegionMarker::getTotalSampleLength());
                                  xmlElement->addChildElement(meta);

                                  // Converts all the regions to XML form.
                                  for(int i = 0; i < RegionObserver::getSize(); i++){
                                      auto rm = RegionObserver::getRegionMarker(i);
                                      auto xmlRegionMarker = new juce::XmlElement("RegionMarker");
                                      xmlRegionMarker->setAttribute("index", i);
                                      xmlRegionMarker->setAttribute("isStart", rm->getStartEndFlag());
                                      xmlRegionMarker->setAttribute("start", rm->getStartSample());
                                      xmlRegionMarker->setAttribute("end", rm->getEndSample());
                                      xmlRegionMarker->setAttribute("loop", rm->getLoopState());
                                      xmlElement->addChildElement(xmlRegionMarker);
                                  }

                                  xmlElement->writeTo(fc.getResult());
                              }
        );
    };

    load.setBounds(0,0,40,20);
    load.setButtonText("load");
    load.setTopRightPosition(700,470);

    // I originally wrote this in the PluginProcessor file but realized it needed the scope of PluginEditor.
    load.onClick = [this]{

        std::cout<<"adad\n";

        chooser = std::make_unique<juce::FileChooser>(
                "Choose a preset to load.", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                "*.jouerxml"
        );

        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
                              {
                                  if(fc.getResult() != juce::File()){
                                      auto fileLoaded = fc.getResult();
                                      auto XML = juce::parseXML(fileLoaded);
                                      std::cout<<XML->toString();
                                      FileHolder::path = XML->getChildByName("PATH")->getAttributeValue(0);
                                      waveformWindow.filesDropped(FileHolder::path,0,0);

                                      auto treeFromXML = juce::ValueTree::fromXml(XML->toString());

                                      this->processorRef.getApvts().replaceState(treeFromXML);
                                      this->processorRef.setADSREnvelope();
                                      this->processorRef.updateModes();

                                      regionOverlay.generateFromXML(*XML);
                                  }
                              }
        );
    };

    FileHolder::fileLabel.attachToComponent(&waveformWindow,false);
}
