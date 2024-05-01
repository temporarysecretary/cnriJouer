
#include "JouerVoice.h"

// This is literally only here for me to see what's happening in the memory buffer
void debugFunction(float currSample, float counter, int startSample, int numSamples){
    std::cout << "Value: ";
    std::cout << currSample;
    std::cout << " ";
    std::cout << "counter: ";
    std::cout << counter;
    std::cout << " ";
    std::cout << "startSample: ";
    std::cout << startSample;
    std::cout << " ";
    std::cout << "Endpoint: ";
    std::cout << numSamples;
    std::cout << " ";
    std::cout << "\n";
}

void JouerVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    // startSample is whatever sample in the buffer we're starting from. It is NOT always zero.

    if(auto* playingSound = static_cast<JouerSound*> (getCurrentlyPlayingSound().get())){

        // Grab the audio of whatever we're playing
        auto audioBuffer =  *playingSound->getAudioData();

        // Get the left and right audio pointers for our input and output
        auto inL = audioBuffer.getReadPointer(0);
        auto inR = audioBuffer.getReadPointer(1);
        auto outL = outputBuffer.getWritePointer(0);
        auto outR = outputBuffer.getWritePointer(1);

        // This is a for loop basically. While we still have samples to write:
        while(numSamples-- >= 0){
            auto adsrGain = adsr.getNextSample();

            // We're making a new copy of our input values because we shouldn't be messing with them directly.
            // We're grabbing whichever sample is at the start of our region, represented by mover.
            auto L = inL[(int) mover];
            auto R = inR[(int) mover];

            // Mode 1 does something weird. I think what it does is it hangs the sample up.
            // So samples 1-23 would all be the same value, samples 24-47, etc...
            if(mode1){
                int stagger = (int) mover - ((int) mover % 24);
                L = inL[stagger];
                R = inR[stagger];
            }

            // Mode 2 boosts and then clips the value in both channels, resulting in a uniquely distorted sound
            if(mode2){
                L = clip(L * 5);
                R = clip(R * 5);
            }

            //Whatever sample we're at in our output buffer, we'll be adding our modulated output
            // Multiplied by whatever stage of ADSR we're at, and the velocity of our note.
            outL[startSample] += L * level * adsrGain;
            outR[startSample] += R * level * adsrGain;

            // On to the next one. We advance mover by our pitchRatio.
            startSample++;
            mover += pitchRatio;

            // If we've moved past the end (looping forward/ not looping) or before the start (looping backwards)
            // or either (pingpong)
            if(mover > end || mover < start)
            {
                // Stop the note outright if the loop's disabled
                if(RegionObserver::getRegionMarker(activeRegion)->getLoopState() == RegionMarker::LOOP_DISABLED) {
                    stopNote(0, false);
                    break;
                }
                // Send us back to the start if we're looping forward continuously
                if(RegionObserver::getRegionMarker(activeRegion)->getLoopState() == RegionMarker::LOOP_FORWARDS){
                    mover = start;
                    break;
                }
                // Send us to the end (start) if we're looping backward continuously
                if(RegionObserver::getRegionMarker(activeRegion)->getLoopState() == RegionMarker::LOOP_BACKWARDS) {
                    mover = end;
                    break;
                }
                // Invert our pitch ratio (if we were going forward, we must've hit the end, so we need to start going backwards)
                // Then send to either start or end
                if(RegionObserver::getRegionMarker(activeRegion)->getLoopState() == RegionMarker::LOOP_PINGPONG){
                    pitchRatio *= -1;
                    if(mover < start) mover = start;
                    if(mover > end) mover = end;
                }
            }

            // If our ADSR envelope has reached its end, we've got no more audio to output, so we end the note
            if(!adsr.isActive()) {
                stopNote(0, false);
            }
        }
    }
}

void JouerVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound,
                           int currentPitchWheelPosition) {
    auto incoming = dynamic_cast<JouerSound*>(sound);

    // If the sound the Synthesizer wants us to play is a JouerSound, we move forward
    if(incoming != nullptr){
        std::cout<<midiNoteNumber;

        // If the midiNoteNumber is in this lower region, then it's being used to change region markers.
        if(midiNoteNumber >= 41 && midiNoteNumber<= 53){
            int index = midiNoteNumber - 41;
            std::cout<<index;

            // Some DAWs and MIDI controllers allow you to input notes lower than 5. Mine doesn't.
            // This block will not run if the region is trying to be changed out of range.
            if(index < RegionObserver::getSize() && index >= 0){
                auto r = RegionObserver::getRegionMarker(index);
                activeRegion = r->getRegion();
            }

            // Don't play anything at all if we're changing regions.
            stopNote(0, false);
        }

        // This is the marker we reference through the rest of the startNote function to get our start and end values.
        auto currentRegionMarker = RegionObserver::getRegionMarker(activeRegion);

        // Debug.
//        std::cout<<"on";
//        std::cout<<"\n";

        // pitchRatio function adapted from https://newt.phys.unsw.edu.au/jw/notes.html
        // We use it here as a percentage to tell the sampler how fast to go
        // So midi note 72 (an octave above 60) will return a pitch ratio of 2, and so, over in renderNextBlock
        // We will be moving TWO samples per loop instead of just one- going twice as fast
        // Likewise, we will be moving at half a sample per loop at midi note 60
        pitchRatio = (float) pow(2,(midiNoteNumber-60)/12.0);
        level = velocity;

        // Sets the start and end positions of this region
        start = currentRegionMarker->getStartSample();
        end = currentRegionMarker->getEndSample();

        // If this region is set to loop backwards, it must start from the end, and the pitch ratio must be negative
        // since we are moving backwards through the audio buffer
        if(RegionObserver::getRegionMarker(activeRegion)->getLoopState() == RegionMarker::LOOP_BACKWARDS){
            pitchRatio *= -1;
            mover = end;
        }
        else
            mover = start;

        // Debug.
//        std::cout << start;
//        std::cout<<"\n";
//        std::cout<<end;
//        std::cout<<"\n";

        // Grab ADSR envelope to apply later, and put it into the start state
        adsr = dynamic_cast<JouerSound*>(sound)->returnADSR();
        adsr.noteOn();
    }
}

void JouerVoice::stopNote(float velocity, bool allowTailOff) {
    if(allowTailOff) {
        // If we're here, it's because we're releasing the note and it's tailing off
        // In this case we're letting the ADSR envelope reach its end before killing the note
//        std::cout<<"off: release";
//        std::cout<<"\n";
        adsr.noteOff();
    }
    else {
        // If we're here, it's because there's no audio left to play. We'll set our ADSR envelope
        // back to the start and tell the audio processor we've finished our note.
//        std::cout<<"off: cut";
//        std::cout<<"\n";
        adsr.reset();
        clearCurrentNote();
    }
}

bool JouerVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<JouerSound*>(sound) != nullptr;
}

void JouerVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void JouerVoice::pitchWheelMoved(int newPitchWheelValue) {

}

float JouerVoice::clip(float value) {
    // Simple utility function. Basically a ceiling and floor function.
    // Has the neat effect of making whatever goes through this extremely distorted.
    // This is the audio phenomenon known as clipping.

    if (value >= 1) return 0.125 + ((rand() % 100 - 50)/10000);
    else if (value <= -1) return -0.125 + ((rand() % 100 - 50)/10000);
    else return value + ((rand() % 100 - 50)/10000);
}

void JouerVoice::updateModes(bool mode1Enabled, bool mode2Enabled) {
    mode1 = mode1Enabled;
    mode2 = mode2Enabled;
}
