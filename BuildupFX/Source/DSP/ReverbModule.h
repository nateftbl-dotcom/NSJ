#pragma once
#include <JuceHeader.h>

/**
 * Reverb module for creating space and atmosphere during buildups
 */
class ReverbModule
{
public:
    ReverbModule() = default;

    void prepare(double sampleRate, int samplesPerBlock, int numChannels)
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = numChannels;

        reverb.prepare(spec);

        // Set default parameters
        updateParameters(0.5f, 0.2f, 0.5f);
    }

    void reset()
    {
        reverb.reset();
    }

    /**
     * Update reverb parameters
     * @param roomSize 0-1
     * @param wetLevel 0-1
     * @param damping 0-1
     */
    void updateParameters(float roomSize, float wetLevel, float damping = 0.5f)
    {
        juce::Reverb::Parameters params;
        params.roomSize = juce::jlimit(0.0f, 1.0f, roomSize);
        params.wetLevel = juce::jlimit(0.0f, 1.0f, wetLevel);
        params.dryLevel = 1.0f - wetLevel; // Maintain constant power
        params.damping = juce::jlimit(0.0f, 1.0f, damping);
        params.width = 1.0f;
        params.freezeMode = 0.0f;

        reverb.setParameters(params);
    }

    /**
     * Process audio through reverb
     */
    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        reverb.process(context);
    }

private:
    juce::dsp::Reverb reverb;
};
