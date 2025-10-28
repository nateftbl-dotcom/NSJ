#pragma once

#include <array>
#include <random>

class SimpleSequencer
{
public:
    static const int NUM_STEPS = 16;
    
    enum GateState
    {
        UNLOCKED,   // Can be randomized
        LOCKED_ON,  // Always plays
        LOCKED_OFF  // Never plays
    };
    
    SimpleSequencer();
    
    // Setup
    void prepare(double sampleRate, double bpm);
    void setBPM(double bpm);
    
    // Processing
    bool processNextSample();  // Returns true if gate should trigger
    
    // Gate control
    void toggleGate(int step);
    void toggleLock(int step);
    void regenerate();
    
    // Getters
    GateState getGateState(int step) const;
    bool isGateActive(int step) const;
    int getCurrentStep() const { return currentStep; }
    
private:
    void calculateStepDuration();
    
    std::array<GateState, NUM_STEPS> gateStates;
    std::array<bool, NUM_STEPS> currentPattern;
    
    int currentStep;
    double sampleRate;
    double hostBPM;
    
    int samplesPerStep;
    int sampleCounter;
    
    bool lastTriggerState;
    
    std::mt19937 randomGenerator;
};
