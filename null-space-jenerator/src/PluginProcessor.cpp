#include "PluginProcessor.h"
#include "PluginEditor.h"

NullSpaceJeneratorAudioProcessor::NullSpaceJeneratorAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , noteLength(2205)  // ~50ms at 44.1kHz (will be updated in prepareToPlay)
    , noteCounter(0)
    , noteIsOn(false)
{
}

NullSpaceJeneratorAudioProcessor::~NullSpaceJeneratorAudioProcessor()
{
}

const juce::String NullSpaceJeneratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NullSpaceJeneratorAudioProcessor::acceptsMidi() const
{
    return false;
}

bool NullSpaceJeneratorAudioProcessor::producesMidi() const
{
    return false;
}

bool NullSpaceJeneratorAudioProcessor::isMidiEffect() const
{
    return false;
}

double NullSpaceJeneratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NullSpaceJeneratorAudioProcessor::getNumPrograms()
{
    return 1;
}

int NullSpaceJeneratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NullSpaceJeneratorAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String NullSpaceJeneratorAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void NullSpaceJeneratorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void NullSpaceJeneratorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    
    synth.prepare(sampleRate);
    sequencer.prepare(sampleRate, 120.0);
    
    // Note length: 50% of step duration
    noteLength = static_cast<int>(sampleRate * 0.05); // 50ms
}

void NullSpaceJeneratorAudioProcessor::releaseResources()
{
}

bool NullSpaceJeneratorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Only mono/stereo output
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void NullSpaceJeneratorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                     juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    
    // Clear the buffer
    buffer.clear();
    
    // Get BPM from host
    if (auto* playHead = getPlayHead())
    {
        if (auto positionInfo = playHead->getPosition())
        {
            if (positionInfo->getBpm().hasValue())
            {
                double bpm = *positionInfo->getBpm();
                sequencer.setBPM(bpm);
            }
        }
    }
    
    int numSamples = buffer.getNumSamples();
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Check if sequencer triggers a gate
        if (sequencer.processNextSample())
        {
            synth.noteOn(60);  // C4
            noteIsOn = true;
            noteCounter = 0;
        }
        
        // Auto note-off after duration
        if (noteIsOn)
        {
            noteCounter++;
            if (noteCounter >= noteLength)
            {
                synth.noteOff();
                noteIsOn = false;
            }
        }
        
        // Generate audio sample
        float audioSample = synth.processSample();
        
        // Write to output
        leftChannel[sample] = audioSample;
        if (rightChannel != nullptr)
            rightChannel[sample] = audioSample;
    }
}

bool NullSpaceJeneratorAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* NullSpaceJeneratorAudioProcessor::createEditor()
{
    return new NullSpaceJeneratorAudioProcessorEditor(*this);
}

void NullSpaceJeneratorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Save sequencer state
    juce::MemoryOutputStream stream(destData, true);
    
    // Save gate states
    for (int i = 0; i < SimpleSequencer::NUM_STEPS; ++i)
    {
        stream.writeInt(static_cast<int>(sequencer.getGateState(i)));
        stream.writeBool(sequencer.isGateActive(i));
    }
}

void NullSpaceJeneratorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Load sequencer state
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    
    // Load gate states
    for (int i = 0; i < SimpleSequencer::NUM_STEPS; ++i)
    {
        int lockState = stream.readInt();
        bool gateActive = stream.readBool();
        
        // Restore lock state by cycling
        while (static_cast<int>(sequencer.getGateState(i)) != lockState)
        {
            sequencer.toggleLock(i);
        }
        
        // Set gate active state if needed
        if (sequencer.isGateActive(i) != gateActive)
        {
            sequencer.toggleGate(i);
        }
    }
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NullSpaceJeneratorAudioProcessor();
}
