#pragma once

#include <cmath>

class SimpleSynth
{
public:
    SimpleSynth();
    
    void prepare(double sampleRate);
    void noteOn(int midiNote);
    void noteOff();
    
    float processSample();
    
    bool isPlaying() const { return playing; }
    
private:
    double phase;
    double phaseIncrement;
    double frequency;
    double sampleRate;
    
    bool playing;
    
    // Simple envelope for click prevention
    float envelope;
    float envelopeIncrement;
    
    static constexpr double PI = 3.14159265358979323846;
};
