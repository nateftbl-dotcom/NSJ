#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

// Custom button component for each step
class StepButton : public juce::Button
{
public:
    StepButton(int stepIndex);
    
    void setLockState(SimpleSequencer::GateState state);
    void setActive(bool active);
    void setCurrentStep(bool isCurrent);
    
protected:
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, 
                     bool shouldDrawButtonAsDown) override;
    
private:
    SimpleSequencer::GateState lockState;
    bool isActive;
    bool isCurrent;
    int stepIndex;
};

// Main editor component
class NullSpaceJeneratorAudioProcessorEditor : public juce::AudioProcessorEditor,
                                                private juce::Timer
{
public:
    NullSpaceJeneratorAudioProcessorEditor(NullSpaceJeneratorAudioProcessor&);
    ~NullSpaceJeneratorAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
private:
    void timerCallback() override;
    void stepButtonClicked(int stepIndex, const juce::ModifierKeys& mods);
    void regenerateButtonClicked();
    
    NullSpaceJeneratorAudioProcessor& audioProcessor;
    
    // UI components
    std::array<std::unique_ptr<StepButton>, SimpleSequencer::NUM_STEPS> stepButtons;
    juce::TextButton regenerateButton;
    juce::Label titleLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NullSpaceJeneratorAudioProcessorEditor)
};
