#pragma once

#include <JuceHeader.h>
#include "DSP/NoiseGenerator.h"
#include "DSP/FilterModule.h"
#include "DSP/ReverbModule.h"
#include "DSP/DelayModule.h"
#include "DSP/DrumEngine.h"
#include "DSP/IntensityMapper.h"

//==============================================================================
/**
 * BuildupFX - A buildup effect plugin for house music production
 */
class BuildupFXAudioProcessor : public juce::AudioProcessor,
                                 public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    BuildupFXAudioProcessor();
    ~BuildupFXAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // Buildup state enum
    enum class BuildupState
    {
        IDLE,       // Not building
        BUILDING,   // Buildup in progress
        PEAK,       // At peak (very brief)
        DROPPING,   // Drop effect
        COOLDOWN    // After drop
    };

    BuildupState getCurrentState() const { return currentState; }
    float getBuildupProgress() const { return buildupProgress; }
    int getBarsRemaining() const;

    // Public parameter tree
    juce::AudioProcessorValueTreeState parameters;

    // Parameter listener callback
    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    //==============================================================================
    // Parameter layout creation
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // State management
    BuildupState currentState = BuildupState::IDLE;
    int selectedBuildupLength = 16; // bars
    double buildupStartPPQ = 0.0;
    float buildupProgress = 0.0f; // 0.0 to 1.0
    bool autoRetrigger = false;

    // Timing & sync
    double sampleRate = 44100.0;
    double currentBPM = 120.0;
    juce::Optional<double> currentPPQ;
    bool isPlaying = false;

    // DSP modules
    NoiseGenerator noiseGenerator;
    FilterModule inputFilter;
    ReverbModule reverbModule;
    DelayModule delayModule;
    DrumEngine drumEngine;

    // Temporary buffers
    juce::AudioBuffer<float> noiseBuffer;
    juce::AudioBuffer<float> drumBuffer;

    // Parameter smoothers
    juce::SmoothedValue<float> noiseIntensitySmooth;
    juce::SmoothedValue<float> drumIntensitySmooth;
    juce::SmoothedValue<float> noiseMixSmooth;
    juce::SmoothedValue<float> drumMixSmooth;
    juce::SmoothedValue<float> outputLevelSmooth;
    juce::SmoothedValue<float> dryWetSmooth;

    // Drop effect state
    float dropProgress = 0.0f;
    int dropType = 0;

    // Methods
    void updateTimingInfo(const juce::Optional<juce::AudioPlayHead::PositionInfo>& posInfo);
    void updateBuildupProgress();
    void triggerBuildup();
    void triggerDrop();
    void processNoiseAndFX(juce::AudioBuffer<float>& buffer);
    void processDrumHits(juce::AudioBuffer<float>& buffer);
    void applyDropEffect(juce::AudioBuffer<float>& buffer);

    // Parameter atomic references
    std::atomic<float>* noiseIntensityParam = nullptr;
    std::atomic<float>* drumIntensityParam = nullptr;
    std::atomic<float>* noiseMixParam = nullptr;
    std::atomic<float>* drumMixParam = nullptr;
    std::atomic<float>* sidechainDuckParam = nullptr;
    std::atomic<float>* humanizeParam = nullptr;
    std::atomic<float>* outputLevelParam = nullptr;
    std::atomic<float>* dryWetParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BuildupFXAudioProcessor)
};
