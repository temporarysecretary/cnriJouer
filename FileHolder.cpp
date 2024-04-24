//
// Created by moonseekr on 2/22/2024.
//

//
// Created by moonseekr on 2/22/2024.
//

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "FileHolder.h"

juce::File FileHolder::activeSample = juce::File();
juce::Label FileHolder::fileLabel;

void FileHolder::setActiveSample(const juce::File& newSample){
    // Set pointer towards file passed in
    FileHolder::activeSample.operator=(newSample);
    std::cout << "File loaded, named " + activeSample.getFileName() + "\n";

    // Grab the file name and apply it to the label
    FileHolder::fileLabel.setText(FileHolder::activeSample.getFileName().toLowerCase(),juce::NotificationType::dontSendNotification);

    // Read the file into memory so that it can be read later
    juce::AudioFormatManager manager;
    manager.registerBasicFormats();
    auto reader = manager.createReaderFor(newSample);
    int numChannels = reader->numChannels;
    activeSampleBuffer.setSize(numChannels, reader->lengthInSamples);
    reader->read(&activeSampleBuffer, 0, reader->lengthInSamples, 0, true, true);

    // Silly debug that'll take the gain of the 5th sample in memory. If it's not 0 then it's probably fine.
    // It could stilll be zero though, so that's stupid.
//    std::cout << activeSampleBuffer.getReadPointer(1)[5];
}

juce::File FileHolder::getActiveSample(){
    // Returns the file loaded into memory.
//    std::cout << "Returning " + activeSample.getFileName() + "\n";
    return FileHolder::activeSample;
}


bool FileHolder::doesSampleExist() {
    if(activeSample == juce::File()) return false;
    else return true;
}

void FileHolder::setPath(const juce::String &string) {
    path = string;
}
