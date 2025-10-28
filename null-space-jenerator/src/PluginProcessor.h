#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "SimpleSequencer.h"
#include "SimpleSynth.h"

class NullSpaceJeneratorAudioProcessor : public juce::AudioProcessor
{
public:
    NullSpaceJeneratorAudioProcessor();
    ~NullSpaceJeneratorAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    // Access to sequencer and synth for UI
    SimpleSequencer& getSequencer() { return sequencer; }
    SimpleSynth& getSynth() { return synth; }

private:
    SimpleSequencer sequencer;
    SimpleSynth synth;
    
    int noteLength;  // How long to hold each note (in samples)
    int noteCounter; // Counter for note duration
    bool noteIsOn;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NullSpaceJeneratorAudioProcessor)
};
