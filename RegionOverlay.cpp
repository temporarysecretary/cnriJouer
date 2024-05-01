

#include "RegionOverlay.h"

RegionOverlay::RegionOverlay(): regionOverlayBounds(400,200) {
    this->setBounds(regionOverlayBounds);

}

// When a file is loaded, we need a region marker at the beginning
void RegionOverlay::initFirstRegionMarker() {
    regionObserver->clear();

    // Creates start position marker
    auto newMarker = (new RegionMarker(0,this->getWidth(), RegionMarker::START));
    newMarker->attach(regionObserver);
    regionObserver->add(newMarker);
    this->addAndMakeVisible(newMarker);
    newMarker->setBounds(0, 0, 5, this->getHeight());

    std::cout<<newMarker->getStartSample();
    std::cout<< " ";
    std::cout<<newMarker->getEndSample();
    std::cout<<"\n";
}

RegionOverlay::~RegionOverlay(){

}

void RegionOverlay::resized(){

}

void RegionOverlay::paint(juce::Graphics &g) {
    g.setColour(juce::Colour(0xFFb6b9ff));
    if(this -> isVisible()){
        paintOnMouseMove(g);
    }
}

void RegionOverlay::mouseMove(const juce::MouseEvent &event){
    if(this->isVisible()){
        mousePosition = event.getPosition();
        this->repaint();
    }
}

void RegionOverlay::mouseExit(const juce::MouseEvent &event){

}

void RegionOverlay::paintOnMouseMove(juce::Graphics &g){
    // Draws a nice little cursor over the overlay
    g.drawVerticalLine(mousePosition.getX(), 0, 400);
}

void RegionOverlay::mouseDown(const juce::MouseEvent &event){
    // Instantiates a new RegionMarker when clicked.
    if(event.mods.isLeftButtonDown()){

        auto newMarker = (new RegionMarker(event.getMouseDownX(),this->getWidth()));\
        newMarker->attach(regionObserver);
        regionObserver->add(newMarker);
        this->addAndMakeVisible(newMarker);
        newMarker->setBounds(event.getMouseDownX(), 0, 5, this->getHeight());

        std::cout<<newMarker->getStartSample();
        std::cout<< " ";
        std::cout<<newMarker->getEndSample();
        std::cout<<"\n";
    }
}

void RegionOverlay::generateFromXML(juce::XmlElement& xml) {
    // Delete all markers
    regionObserver->clear();

    // Get sample length metadata and every RegionMarker entry from XML
    auto length = xml.getChildByName("SAMPLE_META")->getAttributeValue(0).getIntValue();
    auto regions = xml.getChildWithTagNameIterator("RegionMarker");

    for(auto s: regions){
//        std::cout << length;
//        std::cout << "= length\n";
        // Grab RegionMarker attributes
        auto savedRegion = s->getAttributeValue(0).getIntValue();
        auto isStart = s->getAttributeValue(1).getIntValue();
        auto startSam = s->getAttributeValue(2).getIntValue();
        auto endSam = s->getAttributeValue(3).getIntValue();
        auto loopFlag = s->getAttributeValue(4).getIntValue();

        // Make a new Marker
        putNewMarker(savedRegion, isStart, startSam, endSam, loopFlag, length);
    }
}

void RegionOverlay::putNewMarker(int savedRegion, int isStart, int startSam, int endSam, int loopFlag, int totalLength){
    // calculate location based on a percentage
    // startSam / totalLength = % of sample
    // % of sample * width = where the marker is if 0% is all the way to the left and 100% is all the way to the right
    auto location = startSam * this->getWidth() / totalLength;
    std::cout << location;
    std::cout << "= location\n";

    // Makes a new marker using saved data
    auto newMarker = new RegionMarker(savedRegion, isStart, startSam, endSam, loopFlag);
    newMarker->attach(regionObserver);
    regionObserver->add(newMarker);
    newMarker->setBounds(location, 0, 5, this->getHeight());
    addAndMakeVisible(*newMarker);
}

