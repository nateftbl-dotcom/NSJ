#pragma once
#include <JuceHeader.h>

/**
 * Simple delay module with feedback for buildup tension
 */
class DelayModule
{
public:
    DelayModule() = default;

    void prepare(double sampleRate, int samplesPerBlock, int numChannels)
    {
        this->sampleRate = sampleRate;
        this->numChannels = numChannels;

        // Maximum delay time: 2 seconds
        int maxDelaySamples = static_cast<int>(sampleRate * 2.0);

        delayBuffers.clear();
        for (int ch = 0; ch < numChannels; ++ch)
        {
            delayBuffers.add(new juce::AudioBuffer<float>(1, maxDelaySamples));
            delayBuffers[ch]->clear();
        }

        writePosition = 0;
        setDelayTime(0.375f); // Default: dotted eighth note at 120 BPM
    }

    void reset()
    {
        for (auto* buffer : delayBuffers)
            buffer->clear();

        writePosition = 0;
    }

    /**
     * Set delay time in seconds
     */
    void setDelayTime(float timeInSeconds)
    {
        delayTimeSamples = static_cast<int>(timeInSeconds * sampleRate);
    }

    /**
     * Set delay time synced to tempo
     * @param bpm Beats per minute
     * @param division Note division (0.25 = quarter, 0.5 = eighth, etc.)
     */
    void setDelayTimeSync(double bpm, float division = 0.375f)
    {
        float timeInSeconds = (60.0f / static_cast<float>(bpm)) * division;
        setDelayTime(timeInSeconds);
    }

    /**
     * Process delay effect
     * @param buffer Input/output buffer
     * @param feedback Feedback amount (0-1, be careful above 0.9!)
     * @param mix Dry/wet mix (0-1)
     */
    void process(juce::AudioBuffer<float>& buffer, float feedback, float mix)
    {
        feedback = juce::jlimit(0.0f, 0.95f, feedback); // Limit feedback to prevent runaway
        mix = juce::jlimit(0.0f, 1.0f, mix);

        auto numSamples = buffer.getNumSamples();

        for (int ch = 0; ch < juce::jmin(numChannels, buffer.getNumChannels()); ++ch)
        {
            auto* channelData = buffer.getWritePointer(ch);
            auto* delayData = delayBuffers[ch]->getWritePointer(0);
            int delayBufferSize = delayBuffers[ch]->getNumSamples();

            int localWritePos = writePosition;

            for (int i = 0; i < numSamples; ++i)
            {
                // Read from delay
                int readPos = localWritePos - delayTimeSamples;
                if (readPos < 0)
                    readPos += delayBufferSize;

                float delayedSample = delayData[readPos];

                // Write input + feedback to delay
                float inputSample = channelData[i];
                delayData[localWritePos] = inputSample + (delayedSample * feedback);

                // Mix dry and wet
                channelData[i] = inputSample * (1.0f - mix) + delayedSample * mix;

                // Advance write position
                localWritePos++;
                if (localWritePos >= delayBufferSize)
                    localWritePos = 0;
            }
        }

        writePosition += numSamples;
        if (writePosition >= delayBuffers[0]->getNumSamples())
            writePosition = 0;
    }

private:
    juce::OwnedArray<juce::AudioBuffer<float>> delayBuffers;
    double sampleRate = 44100.0;
    int numChannels = 2;
    int writePosition = 0;
    int delayTimeSamples = 0;
};
