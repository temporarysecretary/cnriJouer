//
// Created by moonseekr on 2/22/2024.
//

//
// Created by moonseekr on 2/22/2024.
//

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "FileHolder.h"

juce::File FileHolder::activeSample = juce::File();
juce::Label FileHolder::fileLabel;

void FileHolder::setActiveSample(const juce::File& newSample){
    FileHolder::activeSample.operator=(newSample);
    std::cout << "File loaded, named " + activeSample.getFileName() + "\n";
    FileHolder::fileLabel.setText(FileHolder::activeSample.getFileName().toLowerCase(),juce::NotificationType::dontSendNotification);
}

juce::File FileHolder::getActiveSample(){
    std::cout << "Returning " + activeSample.getFileName() + "\n";
    return FileHolder::activeSample;
}

bool FileHolder::doesSampleExist() {
    if(activeSample == juce::File()) return false;
    else return true;
}