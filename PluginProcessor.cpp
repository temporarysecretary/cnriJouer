#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, juce::Identifier("jouerAPVTS"),
             initParams())
{
    apvts.state.addListener(this);

    mFormatManager.registerBasicFormats();
    for(int i = 0; i < numVoices; i++){
        synth.addVoice(new JouerVoice());
    }
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{


}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);
    adsrEnvelope.setSampleRate(sampleRate);
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if(shouldADSRUpdate) setADSREnvelope();

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

void AudioPluginAudioProcessor::setADSREnvelope() {

    // If there's no file loaded, don't run!!!! This WILL result in an access violation and crash the program!!!!!!
    if(loadedSample != nullptr){
        adsrEnvelope.setParameters(
                juce::ADSR::Parameters(
                        apvts.getRawParameterValue("ATTACK")->load(),
                        apvts.getRawParameterValue("DECAY")->load(),
                        apvts.getRawParameterValue("SUSTAIN")->load(),
                        apvts.getRawParameterValue("RELEASE")->load()));
        adsrEnvelope.setSampleRate(getSampleRate());

        loadedSample->passEnvelope(adsrEnvelope);
        shouldADSRUpdate = false;
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this,
                                               juce::MidiKeyboardComponent(keyboardState,
                                                                                  juce::KeyboardComponentBase::verticalKeyboardFacingRight));
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

void AudioPluginAudioProcessor::loadFile(juce::String path){
    // Receives a call from WaveformWindow when a file is dropped onto it

    synth.clearSounds();
    juce::File file = juce::File(path);
    mFormatReader = mFormatManager.createReaderFor(file);
    juce::BigInteger range;
    range.setRange(0,128,true);
    loadedSample = new JouerSound("Sample", *mFormatReader, range,
                                  60, 0.0, 0.0, 60);

    synth.addSound(loadedSample);
    setADSREnvelope();

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::initParams(){
    // Vector/Array list of Parameters that we neeeeeeeeeeeeeeed
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // Can't do this any other way. We're making these one at a time.
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
                                 ("ATTACK", "Attack", 0.0f, 2.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
                                 ("DECAY", "Decay", 0.0f, 2.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
                                 ("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
                                 ("RELEASE", "release", 0.0f, 2.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>
                                 ("MODE1", "mode1", false));
    parameters.push_back(std::make_unique<juce::AudioParameterBool>
                                 ("MODE2", "mode2", false));

    // Return from beginning to end
    return{parameters.begin(), parameters.end()};
}

juce::AudioProcessorValueTreeState& AudioPluginAudioProcessor::getApvts(){
    return apvts;
}

void AudioPluginAudioProcessor::valueTreePropertyChanged
(juce::ValueTree &tree, const juce::Identifier &id){
    // We set this to true so that it updates the ADSR envelope on the next memory block.
    // After setADSREnvelope() is called, shouldADSRUpdate will be set back to false by that function.
    shouldADSRUpdate = true;

    // Whenever the modes are changed, we'll just send this. Godspeed.
    JouerVoice::updateModes(apvts.getRawParameterValue("MODE1")->load(), apvts.getRawParameterValue("MODE2")->load());
}

void AudioPluginAudioProcessor::saveXML(){
    std::cout<<"yoohoo\n";

    chooser = std::make_unique<juce::FileChooser>(
            "Choose where to save this preset.", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
            "*.jouerxml"
            );

    auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
        {
            auto xmlElement = apvts.state.createXml();
            auto path = juce::XmlElement::createTextElement(FileHolder::path);
            path->setTagName("PATH");
            xmlElement->addChildElement(path);
            xmlElement->writeTo(fc.getResult());
        }
    );
}

void AudioPluginAudioProcessor::loadXML(){
    std::cout<<"adad\n";

    chooser = std::make_unique<juce::FileChooser>(
            "Choose a preset to load.", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
            "*.jouerxml"
    );

    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
                          {
                              auto fileLoaded = fc.getResult();
                              auto XML = juce::parseXML(fileLoaded);
                              std::cout<<XML->toString();
                              FileHolder::setPath(XML->getChildByName("PATH")->getAttributeValue(0));
                          }
    );

}