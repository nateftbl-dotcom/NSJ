#include "SimpleSequencer.h"
#include <cmath>

SimpleSequencer::SimpleSequencer()
    : currentStep(0)
    , sampleRate(44100.0)
    , hostBPM(120.0)
    , samplesPerStep(0)
    , sampleCounter(0)
    , lastTriggerState(false)
{
    // Initialize all gates as unlocked and inactive
    for (int i = 0; i < NUM_STEPS; ++i)
    {
        gateStates[i] = UNLOCKED;
        currentPattern[i] = false;
    }
    
    // Seed random generator
    randomGenerator.seed(std::random_device()());
    
    // Create initial random pattern
    regenerate();
}

void SimpleSequencer::prepare(double newSampleRate, double bpm)
{
    sampleRate = newSampleRate;
    hostBPM = bpm;
    calculateStepDuration();
}

void SimpleSequencer::setBPM(double bpm)
{
    if (bpm > 0.0 && std::abs(bpm - hostBPM) > 0.1)
    {
        hostBPM = bpm;
        calculateStepDuration();
    }
}

void SimpleSequencer::calculateStepDuration()
{
    // 16 steps per bar, 4 beats per bar
    // So each step is 1/16th note = 1/4 beat
    // Duration = (60 / BPM) / 4 seconds per step
    double secondsPerStep = (60.0 / hostBPM) / 4.0;
    samplesPerStep = static_cast<int>(secondsPerStep * sampleRate);
    
    // Ensure minimum step duration
    if (samplesPerStep < 1)
        samplesPerStep = 1;
}

bool SimpleSequencer::processNextSample()
{
    sampleCounter++;
    
    // Check if we need to advance to next step
    if (sampleCounter >= samplesPerStep)
    {
        sampleCounter = 0;
        currentStep = (currentStep + 1) % NUM_STEPS;
        
        // Trigger if current step is active
        if (currentPattern[currentStep])
        {
            lastTriggerState = true;
            return true;
        }
    }
    
    // Only trigger on the first sample of a step
    if (lastTriggerState && sampleCounter > 0)
    {
        lastTriggerState = false;
    }
    
    return false;
}

void SimpleSequencer::toggleGate(int step)
{
    if (step >= 0 && step < NUM_STEPS)
    {
        currentPattern[step] = !currentPattern[step];
    }
}

void SimpleSequencer::toggleLock(int step)
{
    if (step >= 0 && step < NUM_STEPS)
    {
        // Cycle through lock states
        switch (gateStates[step])
        {
            case UNLOCKED:
                gateStates[step] = LOCKED_ON;
                currentPattern[step] = true;
                break;
            case LOCKED_ON:
                gateStates[step] = LOCKED_OFF;
                currentPattern[step] = false;
                break;
            case LOCKED_OFF:
                gateStates[step] = UNLOCKED;
                break;
        }
    }
}

void SimpleSequencer::regenerate()
{
    std::uniform_int_distribution<> dist(0, 1);
    
    for (int i = 0; i < NUM_STEPS; ++i)
    {
        if (gateStates[i] == UNLOCKED)
        {
            // 50% chance to activate
            currentPattern[i] = (dist(randomGenerator) == 1);
        }
        // Locked gates remain unchanged
    }
}

SimpleSequencer::GateState SimpleSequencer::getGateState(int step) const
{
    if (step >= 0 && step < NUM_STEPS)
        return gateStates[step];
    return UNLOCKED;
}

bool SimpleSequencer::isGateActive(int step) const
{
    if (step >= 0 && step < NUM_STEPS)
        return currentPattern[step];
    return false;
}
