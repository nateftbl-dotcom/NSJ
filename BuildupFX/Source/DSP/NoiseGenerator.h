#pragma once
#include <JuceHeader.h>

/**
 * Generates white and pink noise for the buildup effect
 */
class NoiseGenerator
{
public:
    NoiseGenerator()
    {
        reset();
    }

    void prepare(double sampleRate, int samplesPerBlock)
    {
        this->sampleRate = sampleRate;
        reset();
    }

    void reset()
    {
        // Reset pink noise filter states
        for (int i = 0; i < 5; ++i)
            b[i] = 0.0f;
    }

    /**
     * Generate noise into buffer
     * @param buffer The buffer to fill with noise
     * @param whiteLevel Level of white noise (0-1)
     * @param pinkLevel Level of pink noise (0-1)
     * @param pitchShift Pitch shift in semitones (for dramatic effect)
     */
    void process(juce::AudioBuffer<float>& buffer, float whiteLevel, float pinkLevel, float pitchShift = 0.0f)
    {
        auto numSamples = buffer.getNumSamples();
        auto numChannels = buffer.getNumChannels();

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Generate white noise
            float white = random.nextFloat() * 2.0f - 1.0f;

            // Generate pink noise using Paul Kellet's filter
            float pink = generatePinkNoise(white);

            // Mix white and pink
            float noise = (white * whiteLevel) + (pink * pinkLevel);

            // Apply pitch shift if needed (simple speed-based approach)
            if (pitchShift != 0.0f)
            {
                // Add harmonics for pitch rise effect
                float harmonic = std::sin(harmonicPhase) * 0.3f;
                noise = noise * 0.7f + harmonic;

                float pitchRatio = std::pow(2.0f, pitchShift / 12.0f);
                harmonicPhase += juce::MathConstants<float>::twoPi * 440.0f * pitchRatio / sampleRate;

                if (harmonicPhase > juce::MathConstants<float>::twoPi)
                    harmonicPhase -= juce::MathConstants<float>::twoPi;
            }

            // Write to all channels
            for (int ch = 0; ch < numChannels; ++ch)
            {
                buffer.addSample(ch, sample, noise);
            }
        }
    }

private:
    juce::Random random;
    double sampleRate = 44100.0;

    // Pink noise filter states (Paul Kellet's algorithm)
    float b[5] = {0};
    float harmonicPhase = 0.0f;

    float generatePinkNoise(float white)
    {
        b[0] = 0.99886f * b[0] + white * 0.0555179f;
        b[1] = 0.99332f * b[1] + white * 0.0750759f;
        b[2] = 0.96900f * b[2] + white * 0.1538520f;
        b[3] = 0.86650f * b[3] + white * 0.3104856f;
        b[4] = 0.55000f * b[4] + white * 0.5329522f;

        float pink = b[0] + b[1] + b[2] + b[3] + b[4] + white * 0.115926f;
        return pink * 0.11f; // Scale to roughly match white noise level
    }
};
