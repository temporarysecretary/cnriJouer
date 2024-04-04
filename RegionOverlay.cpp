//
// Created by moonseekr on 3/28/2024.
//

#include "RegionOverlay.h"

RegionOverlay::RegionOverlay(): regionOverlayBounds(400,200) {
    this->setBounds(regionOverlayBounds);
}

RegionOverlay::~RegionOverlay(){

}

void RegionOverlay::resized(){

}

void RegionOverlay::paint(juce::Graphics &g) {
    g.setColour(juce::Colour(0xFFb6b9ff));
    if(FileHolder::doesSampleExist()){
        paintOnMouseClick(g);
        paintOnMouseMove(g);
    }
}

void RegionOverlay::mouseMove(const juce::MouseEvent &event){
    mousePosition = event.getPosition();
    this->setVisible(true);
    this->repaint();
}

void RegionOverlay::mouseExit(const juce::MouseEvent &event){
    this->toBack();
    this->setVisible(false);
}

void RegionOverlay::paintOnMouseMove(juce::Graphics &g){
    g.drawVerticalLine(mousePosition.getX(), 0, 400);
}

void RegionOverlay::paintOnMouseClick(juce::Graphics &g){
}