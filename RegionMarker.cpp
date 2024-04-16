//
// Created by moonseekr on 4/1/2024.
//

#include "RegionMarker.h"

RegionMarker::RegionMarker(){
    state = LOOP_DISABLED;
    startSample = 0;
    endSample = 0;
}

RegionMarker::~RegionMarker(){

}

void RegionMarker::paint(juce::Graphics &g){
    switch(state){
        case 0: color.operator=(juce::Colour(0xFF000000)); break;
        case 1: color.operator=(juce::Colour(0xFF00FF00)); break;
        case 2: color.operator=(juce::Colour(0xFF0000FF)); break;
        case 3: color.operator=(juce::Colour(0xFFFF0000)); break;
    }

    std::cout << color.toString();

    int width = 3;
    if(isMouseOver(false)) width = 5;

    g.setColour(color);
    g.fillRect(this->getWidth()/2, 0, width, this->getHeight());
}

void RegionMarker::change(){
    switch(state){
        case 0:// LOOP_DISABLED
                state = LOOP_FORWARDS;
            break;
        case 1:                         // LOOP_FORWARDS
                state = LOOP_BACKWARDS;
            break;
        case 2:                         // LOOP_BACKWARDS
                state = LOOP_PINGPONG;
            break;
        case 3:                         // LOOP_PINGPONG
                state = LOOP_DISABLED;
            break;
    }
    repaint();
}

void RegionMarker::mouseDown(const juce::MouseEvent &e){
    if(e.mods.isLeftButtonDown()){
        change();
    }
    if(e.mods.isRightButtonDown()){
        delete this;
    }
}

void RegionMarker::mouseEnter(const juce::MouseEvent &e){
    repaint();
}

int RegionMarker::getState() const{
    return state;
}

void RegionMarker::attach(RegionObserver *newObserver) {
    this->observer = newObserver;
}

RegionMarker::RegionMarker(int xPos, int full) {
    state = LOOP_DISABLED;
    startSample = xPos;
    endSample = full;
}

