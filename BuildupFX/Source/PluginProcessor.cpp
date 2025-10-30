#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BuildupFXAudioProcessor::BuildupFXAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
    :
#endif
      parameters(*this, nullptr, juce::Identifier("BuildupFX"), createParameterLayout())
{
    // Get parameter pointers for fast access
    noiseIntensityParam = parameters.getRawParameterValue("noiseIntensity");
    drumIntensityParam = parameters.getRawParameterValue("drumIntensity");
    noiseMixParam = parameters.getRawParameterValue("noiseMix");
    drumMixParam = parameters.getRawParameterValue("drumMix");
    sidechainDuckParam = parameters.getRawParameterValue("sidechainDuck");
    humanizeParam = parameters.getRawParameterValue("humanize");
    outputLevelParam = parameters.getRawParameterValue("outputLevel");
    dryWetParam = parameters.getRawParameterValue("dryWet");

    // Listen to trigger parameter
    parameters.addParameterListener("triggerButton", this);
    parameters.addParameterListener("buildupLength", this);
    parameters.addParameterListener("autoRetrigger", this);
    parameters.addParameterListener("dropType", this);
}

BuildupFXAudioProcessor::~BuildupFXAudioProcessor()
{
    parameters.removeParameterListener("triggerButton", this);
    parameters.removeParameterListener("buildupLength", this);
    parameters.removeParameterListener("autoRetrigger", this);
    parameters.removeParameterListener("dropType", this);
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout BuildupFXAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Buildup Control
    layout.add(std::make_unique<juce::AudioParameterChoice>("buildupLength", "Buildup Length",
                                                             juce::StringArray{"4 Bars", "8 Bars", "16 Bars", "32 Bars"},
                                                             2)); // Default: 16 bars

    layout.add(std::make_unique<juce::AudioParameterBool>("triggerButton", "Trigger", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("autoRetrigger", "Auto Retrigger", false));

    // Noise Section
    layout.add(std::make_unique<juce::AudioParameterFloat>("noiseIntensity", "Noise Intensity",
                                                            juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                                                            65.0f));

    layout.add(std::make_unique<juce::AudioParameterChoice>("noisePreset", "Noise Preset",
                                                             juce::StringArray{"Riser", "Dark Tension", "Bright Energy", "Filtered"},
                                                             0)); // Default: Riser

    layout.add(std::make_unique<juce::AudioParameterFloat>("noiseMix", "Noise Mix",
                                                            juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                                                            50.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("sidechainDuck", "Sidechain Duck",
                                                            juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                                                            30.0f));

    // Drum Section
    layout.add(std::make_unique<juce::AudioParameterFloat>("drumIntensity", "Drum Intensity",
                                                            juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                                                            80.0f));

    layout.add(std::make_unique<juce::AudioParameterChoice>("drumPreset", "Drum Preset",
                                                             juce::StringArray{"Snare Roll", "Full Kit", "Minimal", "Kick Build"},
                                                             0)); // Default: Snare Roll

    layout.add(std::make_unique<juce::AudioParameterFloat>("drumMix", "Drum Mix",
                                                            juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                                                            70.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("humanize", "Humanize",
                                                            juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                                                            20.0f));

    // Global Controls
    layout.add(std::make_unique<juce::AudioParameterChoice>("dropType", "Drop Type",
                                                             juce::StringArray{"None", "Vinyl Brake", "Filter Drop", "Impact"},
                                                             0)); // Default: None

    layout.add(std::make_unique<juce::AudioParameterFloat>("outputLevel", "Output Level",
                                                            juce::NormalisableRange<float>(-60.0f, 12.0f, 0.1f),
                                                            0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("dryWet", "Dry/Wet",
                                                            juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f),
                                                            100.0f));

    return layout;
}

//==============================================================================
const juce::String BuildupFXAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BuildupFXAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BuildupFXAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BuildupFXAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BuildupFXAudioProcessor::getTailLengthSeconds() const
{
    return 2.0; // Reverb and delay tail
}

int BuildupFXAudioProcessor::getNumPrograms()
{
    return 1;
}

int BuildupFXAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BuildupFXAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String BuildupFXAudioProcessor::getProgramName(int index)
{
    return {};
}

void BuildupFXAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void BuildupFXAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "triggerButton")
    {
        if (newValue > 0.5f) // Button pressed
        {
            if (currentState == BuildupState::IDLE || currentState == BuildupState::COOLDOWN)
            {
                triggerBuildup();
            }
        }
    }
    else if (parameterID == "buildupLength")
    {
        // Update will be picked up on next trigger
    }
    else if (parameterID == "autoRetrigger")
    {
        autoRetrigger = (newValue > 0.5f);
    }
    else if (parameterID == "dropType")
    {
        dropType = static_cast<int>(newValue);
    }
}

//==============================================================================
void BuildupFXAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;

    // Prepare DSP modules
    noiseGenerator.prepare(sampleRate, samplesPerBlock);
    inputFilter.prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    reverbModule.prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    delayModule.prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    drumEngine.prepare(sampleRate, samplesPerBlock);

    // Allocate temporary buffers
    noiseBuffer.setSize(getTotalNumOutputChannels(), samplesPerBlock);
    drumBuffer.setSize(getTotalNumOutputChannels(), samplesPerBlock);

    // Initialize smoothers
    noiseIntensitySmooth.reset(sampleRate, 0.05); // 50ms ramp
    drumIntensitySmooth.reset(sampleRate, 0.05);
    noiseMixSmooth.reset(sampleRate, 0.05);
    drumMixSmooth.reset(sampleRate, 0.05);
    outputLevelSmooth.reset(sampleRate, 0.05);
    dryWetSmooth.reset(sampleRate, 0.05);

    // Set initial values
    noiseIntensitySmooth.setCurrentAndTargetValue(noiseIntensityParam->load());
    drumIntensitySmooth.setCurrentAndTargetValue(drumIntensityParam->load());
    noiseMixSmooth.setCurrentAndTargetValue(noiseMixParam->load());
    drumMixSmooth.setCurrentAndTargetValue(drumMixParam->load());
    outputLevelSmooth.setCurrentAndTargetValue(outputLevelParam->load());
    dryWetSmooth.setCurrentAndTargetValue(dryWetParam->load());
}

void BuildupFXAudioProcessor::releaseResources()
{
    noiseBuffer.setSize(0, 0);
    drumBuffer.setSize(0, 0);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BuildupFXAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
  #else
    // Support mono and stereo
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Input and output layout must match
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BuildupFXAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Get timing info from host
    if (auto* playHead = getPlayHead())
    {
        if (auto posInfo = playHead->getPosition())
            updateTimingInfo(posInfo);
    }

    // Update buildup progress
    updateBuildupProgress();

    // Update parameter smoothers
    noiseIntensitySmooth.setTargetValue(noiseIntensityParam->load());
    drumIntensitySmooth.setTargetValue(drumIntensityParam->load());
    noiseMixSmooth.setTargetValue(noiseMixParam->load() / 100.0f);
    drumMixSmooth.setTargetValue(drumMixParam->load() / 100.0f);
    outputLevelSmooth.setTargetValue(juce::Decibels::decibelsToGain(outputLevelParam->load()));
    dryWetSmooth.setTargetValue(dryWetParam->load() / 100.0f);

    // Store dry signal
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(buffer);

    // Process based on state
    if (currentState == BuildupState::BUILDING)
    {
        // Calculate effective intensities based on progress
        float effectiveNoiseIntensity = noiseIntensitySmooth.getNextValue() * buildupProgress;
        float effectiveDrumIntensity = drumIntensitySmooth.getNextValue() * buildupProgress;

        // Process noise and FX
        processNoiseAndFX(buffer);

        // Process drum hits
        processDrumHits(buffer);

        // Apply sidechain ducking to input
        float duck = sidechainDuckParam->load() / 100.0f;
        float duckAmount = 1.0f - (duck * buildupProgress);
        buffer.applyGain(duckAmount);
    }
    else if (currentState == BuildupState::DROPPING)
    {
        applyDropEffect(buffer);
    }

    // Mix dry and wet
    float dryWet = dryWetSmooth.getNextValue();
    for (int ch = 0; ch < totalNumOutputChannels; ++ch)
    {
        auto* channelData = buffer.getWritePointer(ch);
        auto* dryData = dryBuffer.getReadPointer(ch % dryBuffer.getNumChannels());

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = dryData[sample] * (1.0f - dryWet) + channelData[sample] * dryWet;
        }
    }

    // Apply output level
    float outputGain = outputLevelSmooth.getNextValue();
    buffer.applyGain(outputGain);
}

//==============================================================================
void BuildupFXAudioProcessor::updateTimingInfo(const juce::Optional<juce::AudioPlayHead::PositionInfo>& posInfo)
{
    if (!posInfo.hasValue())
        return;

    // Get BPM
    if (auto bpm = posInfo->getBpm())
        currentBPM = *bpm;

    // Get PPQ position
    currentPPQ = posInfo->getPpqPosition();

    // Get play state
    isPlaying = posInfo->getIsPlaying();
}

void BuildupFXAudioProcessor::updateBuildupProgress()
{
    if (currentState != BuildupState::BUILDING || !currentPPQ.hasValue())
        return;

    // Calculate progress based on PPQ
    double ppqElapsed = *currentPPQ - buildupStartPPQ;
    double ppqLength = selectedBuildupLength * 4.0; // 4 quarter notes per bar

    buildupProgress = juce::jlimit(0.0f, 1.0f, static_cast<float>(ppqElapsed / ppqLength));

    // Check if buildup is complete
    if (buildupProgress >= 1.0f)
    {
        triggerDrop();
    }
}

int BuildupFXAudioProcessor::getBarsRemaining() const
{
    if (currentState != BuildupState::BUILDING)
        return 0;

    int barsElapsed = static_cast<int>(buildupProgress * selectedBuildupLength);
    return selectedBuildupLength - barsElapsed;
}

void BuildupFXAudioProcessor::triggerBuildup()
{
    if (!currentPPQ.hasValue())
        return;

    // Get selected buildup length
    int lengthIndex = static_cast<int>(*parameters.getRawParameterValue("buildupLength"));
    const int lengths[] = {4, 8, 16, 32};
    selectedBuildupLength = lengths[lengthIndex];

    // Start buildup
    currentState = BuildupState::BUILDING;
    buildupStartPPQ = *currentPPQ;
    buildupProgress = 0.0f;

    DBG("Buildup triggered! Length: " << selectedBuildupLength << " bars");
}

void BuildupFXAudioProcessor::triggerDrop()
{
    currentState = BuildupState::DROPPING;
    dropProgress = 0.0f;

    DBG("Drop triggered!");

    // Reset filters
    inputFilter.reset();
    reverbModule.reset();
    delayModule.reset();
}

void BuildupFXAudioProcessor::processNoiseAndFX(juce::AudioBuffer<float>& buffer)
{
    if (currentState != BuildupState::BUILDING)
        return;

    // Get current intensity (modulated by progress)
    float intensity = noiseIntensitySmooth.getNextValue() * buildupProgress;

    // Get noise balance
    auto [whiteLevel, pinkLevel] = IntensityMapper::getNoiseBalance(intensity);

    // Get pitch shift
    float pitchShift = IntensityMapper::getPitchShift(intensity);

    // Generate noise
    noiseBuffer.clear();
    float noiseLevel = IntensityMapper::getNoiseLevel(intensity);
    noiseGenerator.process(noiseBuffer, whiteLevel * noiseLevel, pinkLevel * noiseLevel, pitchShift);

    // Apply filter sweeps to input
    float hpfCutoff = IntensityMapper::getHPFCutoff(intensity);
    float lpfCutoff = IntensityMapper::getLPFCutoff(intensity);
    inputFilter.setHighPassCutoff(hpfCutoff);
    inputFilter.setLowPassCutoff(lpfCutoff);
    inputFilter.process(buffer);

    // Apply reverb
    float reverbSize = IntensityMapper::getReverbSize(intensity);
    float reverbWet = IntensityMapper::getReverbWet(intensity);
    reverbModule.updateParameters(reverbSize, reverbWet);
    reverbModule.process(buffer);

    // Apply delay
    float delayFeedback = IntensityMapper::getDelayFeedback(intensity);
    float delayMix = IntensityMapper::getDelayMix(intensity);
    delayModule.setDelayTimeSync(currentBPM, 0.375f); // Dotted eighth
    delayModule.process(buffer, delayFeedback, delayMix);

    // Mix noise into buffer
    float noiseMix = noiseMixSmooth.getNextValue();
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        buffer.addFrom(ch, 0, noiseBuffer, ch % noiseBuffer.getNumChannels(), 0, buffer.getNumSamples(), noiseMix);
    }
}

void BuildupFXAudioProcessor::processDrumHits(juce::AudioBuffer<float>& buffer)
{
    if (currentState != BuildupState::BUILDING)
        return;

    // Get current intensity
    float intensity = drumIntensitySmooth.getNextValue() * buildupProgress;

    // Get drum preset
    int drumPreset = static_cast<int>(*parameters.getRawParameterValue("drumPreset"));

    // Get humanize amount
    float humanize = humanizeParam->load();

    // Generate drums
    drumBuffer.clear();
    drumEngine.process(drumBuffer, intensity, drumPreset, currentBPM, currentPPQ, isPlaying, humanize);

    // Mix drums into buffer
    float drumMix = drumMixSmooth.getNextValue();
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        buffer.addFrom(ch, 0, drumBuffer, ch % drumBuffer.getNumChannels(), 0, buffer.getNumSamples(), drumMix);
    }
}

void BuildupFXAudioProcessor::applyDropEffect(juce::AudioBuffer<float>& buffer)
{
    int dropTypeInt = static_cast<int>(*parameters.getRawParameterValue("dropType"));

    switch (dropTypeInt)
    {
        case 0: // None
            currentState = BuildupState::IDLE;
            break;

        case 1: // Vinyl Brake
            {
                // Simple time-stretch effect (not real vinyl brake, but gives the idea)
                dropProgress += 0.02f;
                if (dropProgress >= 1.0f)
                {
                    currentState = BuildupState::IDLE;
                    if (autoRetrigger)
                        triggerBuildup();
                }
                else
                {
                    buffer.applyGain(1.0f - dropProgress);
                }
            }
            break;

        case 2: // Filter Drop
            {
                inputFilter.setHighPassCutoff(20.0f);
                inputFilter.setLowPassCutoff(20000.0f);
                inputFilter.process(buffer);
                currentState = BuildupState::IDLE;
                if (autoRetrigger)
                    triggerBuildup();
            }
            break;

        case 3: // Impact
            {
                // Brief transient punch
                buffer.applyGain(1.5f);
                currentState = BuildupState::IDLE;
                if (autoRetrigger)
                    triggerBuildup();
            }
            break;
    }
}

//==============================================================================
bool BuildupFXAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* BuildupFXAudioProcessor::createEditor()
{
    return new BuildupFXAudioProcessorEditor(*this);
}

//==============================================================================
void BuildupFXAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BuildupFXAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BuildupFXAudioProcessor();
}
