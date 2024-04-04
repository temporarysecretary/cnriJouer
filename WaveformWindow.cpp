//
// Created by moonseekr on 2/5/2024.
//

#include "WaveformWindow.h"//
#include <juce_gui_basics/juce_gui_basics.h>
#include "FileHolder.h"
#include <juce_audio_formats/juce_audio_formats.h>


WaveformWindow::WaveformWindow(AudioPluginAudioProcessor &p) : thumbnailCache(5),
                                   thumbnail(512, formatManager, thumbnailCache),
                                   processorRef(p), waveformWindowBounds(400,200){
    std::cout << "constructing WaveformWindow\n";
    setBounds(waveformWindowBounds);
    setAvailableAudioFormats();
    gainOfThumbnail = 1.0f;
    thumbnail.addChangeListener(this);
}

void WaveformWindow::paint(juce::Graphics &g) {
    std::cout << "we've made it to paint (WW)\n";

    if (thumbnail.isFullyLoaded() == 0)
        paintIfNoFileLoaded (g, waveformWindowBounds);
    else
        paintIfFileLoaded (g, waveformWindowBounds);
}

void WaveformWindow::resized() {
}

void WaveformWindow::changeListenerCallback(juce::ChangeBroadcaster *source) {
    if(source == &thumbnail) thumbnailChanged();
}

void WaveformWindow::thumbnailChanged(){
    this->repaint();
}

void WaveformWindow::setAvailableAudioFormats() {
    std::cout << "we've made it to setAvailable\n";
    formatManager.registerBasicFormats();
}

bool WaveformWindow::isInterestedInFileDrag(const juce::StringArray &files) {
    std::cout << "we've made it to Interest\n";
    if(files.size() > 1) return false;
    else{
        std::cout << "testing Interest\n";
        juce::File tempFile(files[0]);
        if(tempFile.existsAsFile() && formatManager.getWildcardForAllFormats().contains("*" + tempFile.getFileExtension())){
            return true;
        }
        else return false;
    }
}

void WaveformWindow::fileDragEnter(const juce::StringArray &files, int x, int y) {
    this->toFront(false);
}

void WaveformWindow::fileDragExit(const juce::StringArray &files) {
    this->toBack();
}

void WaveformWindow::filesDropped(const juce::StringArray &files, int x, int y) {
    std::cout << "testing Drop\n";
    juce::File tempFile(files[0]);
    if(tempFile.existsAsFile() && formatManager.getWildcardForAllFormats().contains("*" + tempFile.getFileExtension())) {

        std::cout << "file is valid\n";
        FileHolder::setActiveSample(tempFile);
        auto* reader = formatManager.createReaderFor(FileHolder::getActiveSample());
        if(reader != nullptr){
            std::cout << "sending to thumbnail\n";
            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            thumbnail.setSource(new juce::FileInputSource(FileHolder::getActiveSample()));
            processorRef.loadFile(FileHolder::getActiveSample().getFullPathName());

            readerSource.reset(newSource.release());
        }
    }

}

void WaveformWindow::paintIfNoFileLoaded(juce::Graphics &g, const juce::Rectangle<int> bounds) {
    g.setColour(juce::Colours::azure);
    g.fillRect(bounds);
    g.setColour(juce::Colours::mediumvioletred);
    g.drawFittedText("no file!", bounds, juce::Justification::centred, 1);
}

void WaveformWindow::paintIfFileLoaded(juce::Graphics &g, const juce::Rectangle<int> bounds) {
    std::cout << "Starting file load paintover\n";

    g.setColour(juce::Colours::white);
    g.fillRect(bounds);

    // Draw left stereo channel
    g.setColour(juce::Colour(0x55000000));
    thumbnail.drawChannel(g,
                          bounds,
                          0.0,
                          thumbnail.getTotalLength(),
                          0,
                          gainOfThumbnail);

    // Draw right stereo channel
    g.setColour(juce::Colour(0x55000000));
    thumbnail.drawChannel(g,
                          bounds,
                          0.0,
                          thumbnail.getTotalLength(),
                          1,
                          gainOfThumbnail);

}

void WaveformWindow::sliderValueChanged(juce::Slider *slider) {
    gainOfThumbnail = slider->getValue();
    repaint();
}

WaveformWindow::~WaveformWindow() {

}