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
    // Debug so we know where we are in the process of loading in
    //    std::cout << "constructing WaveformWindow\n";

    // Set bounds
    setBounds(waveformWindowBounds);
    // Set our formatManager's formats
    setAvailableAudioFormats();

    // Initialize properties of our audioThumbnail
    gainOfThumbnail = 1.0f;
    thumbnail.addChangeListener(this);
    thumbnailCache.loadThumb(thumbnail,0);

    // Initialize properties of the regionOverlay
    regionOverlay.setBounds(this->getBounds());
    addChildComponent(regionOverlay);
}

void WaveformWindow::paint(juce::Graphics &g) {
//    std::cout << "we've made it to paint (WW)\n";
    // If there are no channels in our thumbnail, there must not be any audio, and no file
    if (!thumbnail.getNumChannels())
        paintIfNoFileLoaded (g, waveformWindowBounds);
    else
        paintIfFileLoaded (g, waveformWindowBounds);
}

void WaveformWindow::resized() {
}

void WaveformWindow::changeListenerCallback(juce::ChangeBroadcaster *source) {
    // If our thumbnail has changed for some reason, we should repaint this component
    if(source == &thumbnail) repaint();
}

void WaveformWindow::setAvailableAudioFormats() {
//    std::cout << "we've made it to setAvailable\n";
    formatManager.registerBasicFormats();
}

bool WaveformWindow::isInterestedInFileDrag(const juce::StringArray &files) {
//    std::cout << "we've made it to Interest\n";

    // You can only load one sample in at a time! Reject the files if there's more than one.
    if(files.size() > 1) return false;
    else{
//        std::cout << "testing Interest\n";

        // If this file is one of our valid formats, we're interested in it.
        juce::File tempFile(files[0]);
        if(tempFile.existsAsFile() && formatManager.getWildcardForAllFormats().contains("*" + tempFile.getFileExtension())){
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
//    std::cout << "testing Drop\n";
    juce::File tempFile(files[0]);

    // If this file exists, and it's valid, we'll load it into memory.
    if(tempFile.existsAsFile() && formatManager.getWildcardForAllFormats().contains("*" + tempFile.getFileExtension())) {
//        std::cout << "file is valid\n";
        // Load the file itself and its path into memory.
        FileHolder::setActiveSample(tempFile);
        FileHolder::setPath(files[0]);

        // Prepare a temporary reader for our file.
        auto* reader = formatManager.createReaderFor(FileHolder::getActiveSample());

        // If it exists:
        if(reader != nullptr){
//            std::cout << "sending to thumbnail\n";
            thumbnailCache.removeThumb(0);


//            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            thumbnail.setSource(new juce::FileInputSource(FileHolder::getActiveSample()));
            processorRef.loadFile(FileHolder::getActiveSample().getFullPathName());

            RegionMarker::setTotalSampleLength(reader->lengthInSamples);
            regionOverlay.initFirstRegionMarker();

//            readerSource.reset(newSource.release());
        }
    }

}

void WaveformWindow::paintIfNoFileLoaded(juce::Graphics &g, const juce::Rectangle<int> bounds) {
    g.setColour(juce::Colours::azure);
    g.fillRect(bounds);
    g.setColour(juce::Colours::mediumvioletred);
    g.drawFittedText("no file!", bounds, juce::Justification::centred, 1);

    if(regionOverlay.isVisible()) regionOverlay.setVisible(false);
}

void WaveformWindow::paintIfFileLoaded(juce::Graphics &g, const juce::Rectangle<int> bounds) {
//    std::cout << "Starting file load paintover\n";

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

    if(!regionOverlay.isVisible()) regionOverlay.setVisible(true);

}

void WaveformWindow::sliderValueChanged(juce::Slider *slider) {
    gainOfThumbnail = slider->getValue();
    repaint();
}

WaveformWindow::~WaveformWindow() {

}