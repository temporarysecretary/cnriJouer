
#include "RegionMarker.h"

// No-arg constructor
RegionMarker::RegionMarker(){
    loopState = LOOP_DISABLED;
    startSample = 0;
    endSample = 0;
}

// On destruction, tell observer that you're going bye-bye
RegionMarker::~RegionMarker(){
    observer->remove(this);
}

// Constructor for use with generateFromXML (see: RegionOverlay)
RegionMarker::RegionMarker(int savedRegion, int isStart, int start, int end, int loopFlag) {
    std::cout<<"made region marker from XML"; // debug
    region = savedRegion;
    startEndFlag = isStart;
    startSample = start;
    endSample = end;
    loopState = loopFlag;
}


void RegionMarker::paint(juce::Graphics &g){
    std::cout<<"painting region marker"; // debug

    // Change colour depending on loop behavior
    switch(loopState){
        case 0: color.operator=(juce::Colour(0xFF000000)); break;
        case 1: color.operator=(juce::Colour(0xFF00FF00)); break;
        case 2: color.operator=(juce::Colour(0xFF0000FF)); break;
        case 3: color.operator=(juce::Colour(0xFFFF0000)); break;
    }

    std::cout << color.toString(); // debug

    // Set colour and fill rectangle
    g.setColour(color);
    g.fillRect(this->getWidth()/2, 0, this->getWidth(), this->getHeight());
}

void RegionMarker::resized(){
    setAlwaysOnTop(true);
    setOpaque(true);
}

void RegionMarker::change(){
    // If you're at one, go to the next, just keep looping, etc...
    switch(loopState){
        case 0:// LOOP_DISABLED
                loopState = LOOP_FORWARDS;
            break;
        case 1:                         // LOOP_FORWARDS
                loopState = LOOP_BACKWARDS;
            break;
        case 2:                         // LOOP_BACKWARDS
                loopState = LOOP_PINGPONG;
            break;
        case 3:                         // LOOP_PINGPONG
                loopState = LOOP_DISABLED;
            break;
    }

    // Change colours too while you're at it
    repaint();
}

void RegionMarker::mouseDown(const juce::MouseEvent &e){
// On left click, change colours
    if(e.mods.isLeftButtonDown()){
        change();
    }
    // On right click, delete, but only if the marker isn't the very
    // first marker
    if(e.mods.isRightButtonDown()){
        if(getStartEndFlag() == NEITHER) delete this;
        else std::cout<<"Attempted deletion of start or endpoint";
    }
}

void RegionMarker::mouseEnter(const juce::MouseEvent &e){
    repaint();
}

int RegionMarker::getLoopState(){
    return loopState;
}

int RegionMarker::getStartEndFlag(){
    return startEndFlag;
}

void RegionMarker::attach(RegionObserver *newObserver) {
    this->observer = newObserver;
}

// Constructor without flag
RegionMarker::RegionMarker(int xPos, int full) {
    loopState = LOOP_DISABLED;
    startEndFlag = NEITHER;

    float percent = (float) xPos / full;
    startSample = percent * totalSampleLength;
    endSample = totalSampleLength;
}

// Constructor for initFirst in RegionOverlay
RegionMarker::RegionMarker(int xPos, int full, int flag) {
    loopState = LOOP_DISABLED;
    startEndFlag = flag;

    float percent = (float) xPos / full;
    startSample = percent * totalSampleLength;
    endSample = totalSampleLength;
}

int RegionMarker::getStartSample() {
    return startSample;
}

int RegionMarker::getEndSample() {
    return endSample;
}

void RegionMarker::setTotalSampleLength(int newLength) {
    totalSampleLength = newLength;
}

int RegionMarker::getTotalSampleLength() {
    return totalSampleLength;
}

void RegionMarker::setStartSample(int newStart) {
    startSample = newStart;
}

void RegionMarker::setEndSample(int newEnd) {
    endSample = newEnd;
}

int RegionMarker::getRegion() {
    return region;
}

void RegionMarker::setRegion(int newRegion) {
    region = newRegion;
}



// RegionObserver class

RegionObserver::RegionObserver(){
    size = 0;
}

// Add a new RegionMarker and sort
void RegionObserver::add(RegionMarker *r) {
    regionMarkers.push_back(r);
    r->setRegion(size);
    size++;
    if(regionMarkers.size() > 1){
        sort();
        update();
    }
}

// Clear all RegionMarkers by deleting them, then clear the array
// I'm told this is incredibly stupid and bad by some senior programmers,
// but I honestly don't know how to do this better
void RegionObserver::clear() {
    std::cout<< "clearing observer\n";

    while(regionMarkers.begin() != regionMarkers.end()){
        std::cout<< "deleting marker\n";
        delete regionMarkers[0];
    }

    std::cout<< "calling vector clear";
    regionMarkers.clear();
    size = 0;
}

// When a new marker is made, change the endpoint of the marker before it to the start of this one
void RegionObserver::update() {
    std::cout<<"updating\n";

    for(int i = 1; i<regionMarkers.size(); i++){
        regionMarkers[i-1]->setEndSample(regionMarkers[i]->getStartSample());
//        std::cout<<i-1;
//        std::cout<<" ";
//        std::cout<<regionMarkers[i-1]->getStartSample();
//        std::cout<<" ";
//        std::cout<<regionMarkers[i-1]->getEndSample();
//        std::cout<<"\n";
    }

    auto end = regionMarkers[regionMarkers.size()-1];
//    std::cout<<"end: ";
//    std::cout<<end->getStartSample();
//    std::cout<<" ";
//    std::cout<<end->getEndSample();
//    std::cout<<"\n";
}

void RegionObserver::sort() {
    std::cout<<"sorting\n";
    bool swapped;

    for(auto j: regionMarkers){
//        std::cout<<"unsorted: ";
//        std::cout<<j->getStartSample();
//        std::cout<<"\n";
    }

    // Simple bubble sort algorithm
    do{
        swapped = false;
        for(int i = 1; i < regionMarkers.size(); i++){
            if(regionMarkers[i]->getStartSample() < regionMarkers[i-1]->getStartSample()){
                auto swap = regionMarkers[i];
                regionMarkers[i] = regionMarkers[i-1];
                regionMarkers[i-1] = swap;
                swapped = true;
                break;
            }
        }
    } while(swapped);


    for(auto j: regionMarkers){
//        std::cout<<"sorted: ";
//        std::cout<<j->getStartSample();
//        std::cout<<"\n";
    }
}

// Deletes the Marker and updates endpoints.
void RegionObserver::remove(RegionMarker* goner){
    std::cout<< "remove called\n";
    auto it = std::find(regionMarkers.begin(), regionMarkers.end(),goner);

    if(it != regionMarkers.begin()){
        std::cout<<"there are other elements in the array\n";
        auto back = *std::prev(it);
        back->setEndSample(goner->getEndSample());
    }

    std::cout<<"the element we want to remove exists, so we'll erase it\n";
    if(it != regionMarkers.end()) regionMarkers.erase(it);
    size--;
}

int RegionObserver::getSize(){
    return size;
}

RegionMarker* RegionObserver::getRegionMarker(int index){
    if(index > size) return regionMarkers[0];
    else return regionMarkers[index];
}