#pragma once
#include <JuceHeader.h>
#include "IntensityMapper.h"

/**
 * Simple drum sample player voice
 */
class DrumVoice
{
public:
    DrumVoice() = default;

    void setSample(const juce::AudioBuffer<float>* sample)
    {
        this->sample = sample;
        position = 0;
        isPlaying = false;
    }

    void trigger(float velocity = 1.0f, float pitchShift = 0.0f)
    {
        if (sample != nullptr)
        {
            position = 0;
            this->velocity = velocity;
            this->pitchRatio = std::pow(2.0f, pitchShift / 12.0f);
            isPlaying = true;
        }
    }

    void stop()
    {
        isPlaying = false;
    }

    bool getIsPlaying() const { return isPlaying; }

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
    {
        if (!isPlaying || sample == nullptr)
            return;

        auto numChannels = juce::jmin(outputBuffer.getNumChannels(), sample->getNumChannels());

        for (int i = 0; i < numSamples; ++i)
        {
            if (position >= sample->getNumSamples())
            {
                isPlaying = false;
                break;
            }

            for (int ch = 0; ch < numChannels; ++ch)
            {
                float sampleValue = sample->getSample(ch % sample->getNumChannels(), static_cast<int>(position));
                outputBuffer.addSample(ch, startSample + i, sampleValue * velocity);
            }

            position += pitchRatio;
        }
    }

private:
    const juce::AudioBuffer<float>* sample = nullptr;
    double position = 0.0;
    float velocity = 1.0f;
    float pitchRatio = 1.0f;
    bool isPlaying = false;
};

/**
 * Drum engine that generates buildup drum patterns
 */
class DrumEngine
{
public:
    DrumEngine()
    {
        // Initialize voices
        for (int i = 0; i < maxVoices; ++i)
            voices.add(new DrumVoice());
    }

    void prepare(double sampleRate, int samplesPerBlock)
    {
        this->sampleRate = sampleRate;
        createDefaultSamples();
    }

    void reset()
    {
        for (auto* voice : voices)
            voice->stop();
    }

    /**
     * Process drum hits for this block
     * @param outputBuffer Buffer to render drums into
     * @param drumIntensity 0-100
     * @param drumPreset 0=Snare Roll, 1=Full Kit, 2=Minimal, 3=Kick Build
     * @param currentBPM Host tempo
     * @param ppqPosition Current position in quarter notes
     * @param isPlaying Whether transport is playing
     * @param humanize 0-100 velocity/timing randomization
     */
    void process(juce::AudioBuffer<float>& outputBuffer,
                 float drumIntensity,
                 int drumPreset,
                 double currentBPM,
                 juce::Optional<double> ppqPosition,
                 bool isPlaying,
                 float humanize)
    {
        if (!isPlaying || !ppqPosition.hasValue())
            return;

        int numSamples = outputBuffer.getNumSamples();
        double samplesPerQuarterNote = (60.0 / currentBPM) * sampleRate;

        // Determine density level
        int densityLevel = IntensityMapper::getDrumDensityLevel(drumIntensity);

        // Calculate hits for this block
        for (int sample = 0; sample < numSamples; ++sample)
        {
            double currentPPQ = *ppqPosition + (sample / samplesPerQuarterNote);

            // Check if we should trigger a hit at this sample
            if (shouldTriggerHit(currentPPQ, densityLevel, drumPreset))
            {
                triggerDrumHit(drumPreset, drumIntensity, humanize);
                lastTriggerPPQ = currentPPQ;
            }
        }

        // Render all active voices
        for (auto* voice : voices)
        {
            if (voice->getIsPlaying())
                voice->renderNextBlock(outputBuffer, 0, numSamples);
        }
    }

private:
    static constexpr int maxVoices = 16;
    juce::OwnedArray<DrumVoice> voices;
    double sampleRate = 44100.0;
    juce::Random random;

    // Sample storage
    juce::AudioBuffer<float> snareSample;
    juce::AudioBuffer<float> kickSample;
    juce::AudioBuffer<float> clapSample;
    juce::AudioBuffer<float> tomSample;

    double lastTriggerPPQ = -1.0;

    /**
     * Create simple synthesized drum samples
     * In a real plugin, you'd load WAV files from BinaryData
     */
    void createDefaultSamples()
    {
        int sampleLength = static_cast<int>(sampleRate * 0.15); // 150ms samples

        // Create synthetic snare
        snareSample.setSize(2, sampleLength);
        snareSample.clear();
        for (int i = 0; i < sampleLength; ++i)
        {
            float t = static_cast<float>(i) / sampleLength;
            float envelope = std::exp(-t * 8.0f);

            // Noise component (snare buzz)
            float noise = (random.nextFloat() * 2.0f - 1.0f) * 0.6f;

            // Tonal component (snare body)
            float tone = std::sin(juce::MathConstants<float>::twoPi * 200.0f * i / sampleRate);
            tone += std::sin(juce::MathConstants<float>::twoPi * 330.0f * i / sampleRate) * 0.5f;

            float sample = (noise * 0.7f + tone * 0.3f) * envelope;

            snareSample.setSample(0, i, sample);
            snareSample.setSample(1, i, sample);
        }

        // Create synthetic kick
        kickSample.setSize(2, sampleLength);
        kickSample.clear();
        for (int i = 0; i < sampleLength; ++i)
        {
            float t = static_cast<float>(i) / sampleLength;
            float envelope = std::exp(-t * 6.0f);

            // Pitch envelope for kick
            float freq = 60.0f + (120.0f * std::exp(-t * 12.0f));
            float sample = std::sin(juce::MathConstants<float>::twoPi * freq * i / sampleRate) * envelope;

            kickSample.setSample(0, i, sample * 1.2f);
            kickSample.setSample(1, i, sample * 1.2f);
        }

        // Create synthetic clap
        clapSample.setSize(2, sampleLength);
        clapSample.clear();
        for (int i = 0; i < sampleLength; ++i)
        {
            float t = static_cast<float>(i) / sampleLength;
            float envelope = 0.0f;

            // Multiple attack segments for clap
            if (i < sampleLength * 0.05f)
                envelope = std::exp(-t * 40.0f);
            else if (i < sampleLength * 0.08f)
                envelope = std::exp(-(t - 0.05f) * 35.0f) * 0.6f;
            else
                envelope = std::exp(-(t - 0.08f) * 30.0f) * 0.4f;

            float noise = (random.nextFloat() * 2.0f - 1.0f);
            float sample = noise * envelope * 0.8f;

            clapSample.setSample(0, i, sample);
            clapSample.setSample(1, i, sample);
        }

        // Create synthetic tom
        tomSample.setSize(2, sampleLength);
        tomSample.clear();
        for (int i = 0; i < sampleLength; ++i)
        {
            float t = static_cast<float>(i) / sampleLength;
            float envelope = std::exp(-t * 7.0f);

            float freq = 100.0f + (80.0f * std::exp(-t * 8.0f));
            float sample = std::sin(juce::MathConstants<float>::twoPi * freq * i / sampleRate) * envelope;

            tomSample.setSample(0, i, sample);
            tomSample.setSample(1, i, sample);
        }
    }

    /**
     * Determine if a hit should be triggered at this PPQ position
     */
    bool shouldTriggerHit(double currentPPQ, int densityLevel, int drumPreset)
    {
        // Prevent retriggering the same position
        if (std::abs(currentPPQ - lastTriggerPPQ) < 0.01)
            return false;

        double beatPosition = std::fmod(currentPPQ, 4.0); // Position within current bar
        double barPosition = std::fmod(currentPPQ / 4.0, 1.0); // Position within bar count

        switch (densityLevel)
        {
            case 0: // Every 2 bars
                return std::abs(std::fmod(currentPPQ, 8.0)) < 0.01;

            case 1: // Every bar
                return std::abs(std::fmod(currentPPQ, 4.0)) < 0.01;

            case 2: // Quarter notes
                return std::abs(std::fmod(currentPPQ, 1.0)) < 0.01;

            case 3: // Eighth notes
                return std::abs(std::fmod(currentPPQ, 0.5)) < 0.01;

            case 4: // Sixteenth notes
                return std::abs(std::fmod(currentPPQ, 0.25)) < 0.01;

            case 5: // Thirty-second notes
                return std::abs(std::fmod(currentPPQ, 0.125)) < 0.01;

            default:
                return false;
        }
    }

    /**
     * Trigger a drum hit
     */
    void triggerDrumHit(int drumPreset, float drumIntensity, float humanize)
    {
        // Find an inactive voice
        DrumVoice* voiceToUse = nullptr;
        for (auto* voice : voices)
        {
            if (!voice->getIsPlaying())
            {
                voiceToUse = voice;
                break;
            }
        }

        if (voiceToUse == nullptr)
            return; // No voices available

        // Get base velocity
        float velocity = IntensityMapper::getDrumVelocity(drumIntensity);

        // Apply humanization
        if (humanize > 0.0f)
        {
            float randomness = (random.nextFloat() * 2.0f - 1.0f) * (humanize / 100.0f) * 0.3f;
            velocity = juce::jlimit(0.3f, 1.0f, velocity + randomness);
        }

        // Get pitch shift
        float pitchShift = IntensityMapper::getDrumPitchShift(drumIntensity);

        // Select sample based on preset
        const juce::AudioBuffer<float>* sampleToPlay = &snareSample;

        switch (drumPreset)
        {
            case 0: // Snare Roll (default)
                sampleToPlay = &snareSample;
                break;

            case 1: // Full Kit (mix it up)
                {
                    int choice = random.nextInt(4);
                    if (choice == 0) sampleToPlay = &kickSample;
                    else if (choice == 1) sampleToPlay = &snareSample;
                    else if (choice == 2) sampleToPlay = &clapSample;
                    else sampleToPlay = &tomSample;
                }
                break;

            case 2: // Minimal (just snare)
                sampleToPlay = &snareSample;
                velocity *= 0.7f; // Quieter
                break;

            case 3: // Kick Build
                sampleToPlay = &kickSample;
                break;
        }

        voiceToUse->setSample(sampleToPlay);
        voiceToUse->trigger(velocity, pitchShift);
    }
};
