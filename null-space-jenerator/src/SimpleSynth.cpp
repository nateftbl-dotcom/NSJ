#include "SimpleSynth.h"

SimpleSynth::SimpleSynth()
    : phase(0.0)
    , phaseIncrement(0.0)
    , frequency(440.0)
    , sampleRate(44100.0)
    , playing(false)
    , envelope(0.0f)
    , envelopeIncrement(0.0f)
{
}

void SimpleSynth::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
}

void SimpleSynth::noteOn(int midiNote)
{
    // Convert MIDI note to frequency
    // A4 (440 Hz) is MIDI note 69
    frequency = 440.0 * std::pow(2.0, (midiNote - 69) / 12.0);
    
    // Calculate phase increment
    phaseIncrement = frequency / sampleRate;
    
    playing = true;
    
    // Quick fade in to prevent clicks (10ms attack)
    envelopeIncrement = 1.0f / (sampleRate * 0.01);
}

void SimpleSynth::noteOff()
{
    // Quick fade out (10ms release)
    envelopeIncrement = -1.0f / (sampleRate * 0.01);
}

float SimpleSynth::processSample()
{
    if (!playing && envelope <= 0.0f)
        return 0.0f;
    
    // Update envelope
    envelope += envelopeIncrement;
    
    // Clamp envelope
    if (envelope > 1.0f)
    {
        envelope = 1.0f;
        envelopeIncrement = 0.0f;
    }
    else if (envelope < 0.0f)
    {
        envelope = 0.0f;
        envelopeIncrement = 0.0f;
        playing = false;
        return 0.0f;
    }
    
    // Generate sine wave
    float output = static_cast<float>(std::sin(phase * 2.0 * PI));
    
    // Advance phase
    phase += phaseIncrement;
    
    // Wrap phase to prevent accumulation
    if (phase >= 1.0)
        phase -= 1.0;
    
    // Apply envelope and reduce volume
    return output * envelope * 0.3f;
}
