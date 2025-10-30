#pragma once
#include <JuceHeader.h>

/**
 * Dual high-pass and low-pass filter module with smooth parameter changes
 */
class FilterModule
{
public:
    FilterModule() = default;

    void prepare(double sampleRate, int samplesPerBlock, int numChannels)
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = numChannels;

        highPassFilter.prepare(spec);
        lowPassFilter.prepare(spec);

        highPassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        lowPassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

        // Set initial cutoffs
        highPassFilter.setCutoffFrequency(20.0f);
        lowPassFilter.setCutoffFrequency(20000.0f);

        // Set resonance (Q)
        highPassFilter.setResonance(0.707f); // Butterworth
        lowPassFilter.setResonance(0.707f);
    }

    void reset()
    {
        highPassFilter.reset();
        lowPassFilter.reset();
    }

    /**
     * Set the high-pass filter cutoff frequency
     */
    void setHighPassCutoff(float frequency)
    {
        highPassFilter.setCutoffFrequency(juce::jlimit(20.0f, 20000.0f, frequency));
    }

    /**
     * Set the low-pass filter cutoff frequency
     */
    void setLowPassCutoff(float frequency)
    {
        lowPassFilter.setCutoffFrequency(juce::jlimit(20.0f, 20000.0f, frequency));
    }

    /**
     * Process audio through both filters
     */
    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        // Apply high-pass first, then low-pass
        highPassFilter.process(context);
        lowPassFilter.process(context);
    }

private:
    juce::dsp::StateVariableTPTFilter<float> highPassFilter;
    juce::dsp::StateVariableTPTFilter<float> lowPassFilter;
};
