#pragma once
#include <JuceHeader.h>
#include <cmath>

/**
 * Maps intensity values (0-100) to various DSP parameters with appropriate curves
 */
class IntensityMapper
{
public:
    // Noise/FX Section Mappings

    /**
     * Get noise generator level (exponential curve for dramatic effect)
     * @param intensity 0-100
     * @return 0-1 level
     */
    static float getNoiseLevel(float intensity)
    {
        float normalized = juce::jlimit(0.0f, 100.0f, intensity) / 100.0f;
        return std::pow(normalized, 2.0f); // Exponential
    }

    /**
     * Get white vs pink noise balance
     * @param intensity 0-100
     * @return whiteLevel (0-1) and pinkLevel (0-1) as a pair
     */
    static std::pair<float, float> getNoiseBalance(float intensity)
    {
        float normalized = intensity / 100.0f;

        if (normalized < 0.3f)
        {
            // Low: mostly white
            return {normalized * 2.0f, normalized * 0.5f};
        }
        else if (normalized < 0.7f)
        {
            // Medium: blend in pink
            float t = (normalized - 0.3f) / 0.4f;
            return {0.6f + t * 0.3f, t * 0.5f};
        }
        else
        {
            // High: more pink for thickness
            float t = (normalized - 0.7f) / 0.3f;
            return {0.9f, 0.5f + t * 0.5f};
        }
    }

    /**
     * Get high-pass filter cutoff frequency
     * LOW (0-30%): 20-200 Hz
     * MED (30-70%): 200-1000 Hz
     * HIGH (70-100%): 1000-8000 Hz
     */
    static float getHPFCutoff(float intensity)
    {
        intensity = juce::jlimit(0.0f, 100.0f, intensity);

        if (intensity < 30.0f)
        {
            float t = intensity / 30.0f;
            return 20.0f + (180.0f * t);
        }
        else if (intensity < 70.0f)
        {
            float t = (intensity - 30.0f) / 40.0f;
            return 200.0f + (800.0f * t);
        }
        else
        {
            float t = (intensity - 70.0f) / 30.0f;
            return 1000.0f + (7000.0f * std::pow(t, 1.5f));
        }
    }

    /**
     * Get low-pass filter cutoff frequency
     * Only active above 70% intensity for bracket effect
     */
    static float getLPFCutoff(float intensity)
    {
        intensity = juce::jlimit(0.0f, 100.0f, intensity);

        if (intensity < 70.0f)
            return 20000.0f; // Fully open

        float t = (intensity - 70.0f) / 30.0f;
        return 20000.0f - (16000.0f * std::pow(t, 2.0f));
    }

    /**
     * Get reverb room size (0.3 to 1.0)
     */
    static float getReverbSize(float intensity)
    {
        float normalized = juce::jlimit(0.0f, 100.0f, intensity) / 100.0f;
        return 0.3f + (0.7f * normalized);
    }

    /**
     * Get reverb wet level (0 to 0.8 max)
     */
    static float getReverbWet(float intensity)
    {
        float normalized = juce::jlimit(0.0f, 100.0f, intensity) / 100.0f;
        return normalized * 0.8f; // Max 80% wet at peak
    }

    /**
     * Get delay feedback amount (0.3 to 0.9)
     */
    static float getDelayFeedback(float intensity)
    {
        intensity = juce::jlimit(0.0f, 100.0f, intensity);

        if (intensity < 30.0f)
            return 0.0f; // No delay at low intensity

        if (intensity < 70.0f)
        {
            float t = (intensity - 30.0f) / 40.0f;
            return 0.3f + (0.3f * t);
        }
        else
        {
            float t = (intensity - 70.0f) / 30.0f;
            return 0.6f + (0.3f * t); // Max 0.9
        }
    }

    /**
     * Get delay mix amount
     */
    static float getDelayMix(float intensity)
    {
        intensity = juce::jlimit(0.0f, 100.0f, intensity);

        if (intensity < 30.0f)
            return 0.0f;

        float t = (intensity - 30.0f) / 70.0f;
        return t * 0.4f; // Max 40% wet
    }

    /**
     * Get pitch shift amount in semitones (0 to +12)
     */
    static float getPitchShift(float intensity)
    {
        intensity = juce::jlimit(0.0f, 100.0f, intensity);

        if (intensity < 30.0f)
            return 0.0f;

        if (intensity < 70.0f)
        {
            float t = (intensity - 30.0f) / 40.0f;
            return t * 2.0f; // 0 to +2 semitones
        }
        else
        {
            float t = (intensity - 70.0f) / 30.0f;
            return 2.0f + (10.0f * t); // +2 to +12 semitones
        }
    }

    // Drum Section Mappings

    /**
     * Determine drum hit density based on intensity
     * Returns: 0=every 2 bars, 1=every bar, 2=quarter, 3=eighth, 4=sixteenth, 5=thirty-second
     */
    static int getDrumDensityLevel(float intensity)
    {
        intensity = juce::jlimit(0.0f, 100.0f, intensity);

        if (intensity < 30.0f)
            return 0; // Every 2 bars
        else if (intensity < 70.0f)
            return 1; // Every bar
        else if (intensity < 80.0f)
            return 2; // Quarter notes
        else if (intensity < 90.0f)
            return 3; // Eighth notes
        else if (intensity < 95.0f)
            return 4; // Sixteenth notes
        else
            return 5; // Thirty-second notes
    }

    /**
     * Get drum velocity (0-1)
     */
    static float getDrumVelocity(float intensity)
    {
        float normalized = juce::jlimit(0.0f, 100.0f, intensity) / 100.0f;
        return 0.6f + (0.4f * normalized); // 0.6 to 1.0
    }

    /**
     * Get drum pitch shift for high-intensity rolls
     */
    static float getDrumPitchShift(float intensity)
    {
        if (intensity < 95.0f)
            return 0.0f;

        float t = (intensity - 95.0f) / 5.0f;
        return t * 4.0f; // 0 to +4 semitones
    }
};
