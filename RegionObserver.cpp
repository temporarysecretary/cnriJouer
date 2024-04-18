
#include "RegionObserver.h"

RegionObserver::RegionObserver(){

}

void RegionObserver::add(RegionMarker *r) {
    regionMarkers.push_back(r);
    sort();
    update();
}

void RegionObserver::update() {

}

void RegionObserver::sort() {
    for(int i = 0; i<regionMarkers.size(); i++){
    }
}

RegionMarker* RegionObserver::getRegionMarker(int index){
    return regionMarkers[index];
}