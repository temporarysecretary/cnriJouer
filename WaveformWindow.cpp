//
// Created by moonseekr on 2/5/2024.
//

#include "WaveformWindow.h"//
#include <juce_gui_basics/juce_gui_basics.h>
#include "FileHolder.h"
#include <juce_audio_formats/juce_audio_formats.h>

juce::Rectangle<int> waveformWindowBounds(400,200);

WaveformWindow::WaveformWindow(AudioPluginAudioProcessor &p) : thumbnailCache(5),
                                   thumbnail(512, testingProblem, thumbnailCache),
                                   processorRef(p){
    std::cout << "constructing WaveformWindow\n";
    setBounds(waveformWindowBounds);
    setAvailableAudioFormats();
    gainOfThumbnail = 1.0f;
    thumbnail.addChangeListener(this);
}

void WaveformWindow::paint(juce::Graphics &g) {
    std::cout << "we've made it to paint (WW)\n";

    if (thumbnail.getNumChannels() == 0 || !fileLoadedIntoThumbnail)
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
    testingProblem.registerBasicFormats();
}

bool WaveformWindow::isInterestedInFileDrag(const juce::StringArray &files) {
    std::cout << "we've made it to Interest\n";
    if(files.size() > 1) return false;
    else{
        std::cout << "testing Interest\n";
        juce::File tempFile(files[0]);
        if(tempFile.existsAsFile() && testingProblem.getWildcardForAllFormats().contains("*" + tempFile.getFileExtension())){
            return true;
        }
        else return false;
    }
}

void WaveformWindow::fileDragEnter(const juce::StringArray &files, int x, int y) {

}

void WaveformWindow::fileDragExit(const juce::StringArray &files) {

}

void WaveformWindow::filesDropped(const juce::StringArray &files, int x, int y) {
    std::cout << "testing Drop\n";
    juce::File tempFile(files[0]);
    if(tempFile.existsAsFile() && testingProblem.getWildcardForAllFormats().contains("*" + tempFile.getFileExtension())) {

        std::cout << "file is valid\n";
        FileHolder::setActiveSample(tempFile);
        auto* reader = testingProblem.createReaderFor(FileHolder::getActiveSample());
        if(reader != nullptr){
            std::cout << "sending to thumbnail\n";
            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            thumbnail.setSource(new juce::FileInputSource(FileHolder::getActiveSample()));
            processorRef.loadFile(FileHolder::getActiveSample().getFullPathName());
            fileLoadedIntoThumbnail = true;
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

    g.setColour(juce::Colours::azure);
    g.fillRect(bounds);

    // Draw left stereo channel
    g.setColour(juce::Colour(0x550099FF));
    thumbnail.drawChannel(g,
                          bounds,
                          0.0,
                          thumbnail.getTotalLength(),
                          0,
                          gainOfThumbnail);

    // Draw right stereo channel
    g.setColour(juce::Colour(0x55FF9900));
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

void WaveformWindow::mouseMove(const juce::MouseEvent &event, juce::Graphics &g){
    paintOverChildren(g, event.getPosition().getX());
}

void WaveformWindow::paintOverChildren(juce::Graphics &g, int mouseX) {
    g.drawVerticalLine(mouseX, 0, waveformWindowBounds.getHeight());
}

WaveformWindow::~WaveformWindow() {

}