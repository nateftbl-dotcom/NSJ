#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/CustomLookAndFeel.h"

//==============================================================================
/**
 * BuildupFX Editor
 */
class BuildupFXAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       public juce::Timer
{
public:
    BuildupFXAudioProcessorEditor(BuildupFXAudioProcessor&);
    ~BuildupFXAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    BuildupFXAudioProcessor& audioProcessor;
    BuildupLookAndFeel customLookAndFeel;

    // UI Components
    // Buildup length radio buttons
    juce::ToggleButton length4Button {"4"};
    juce::ToggleButton length8Button {"8"};
    juce::ToggleButton length16Button {"16"};
    juce::ToggleButton length32Button {"32"};

    juce::ToggleButton autoRetriggerButton {"Auto-Retrigger"};
    juce::TextButton triggerButton {"TRIGGER"};

    // Intensity knobs
    juce::Slider noiseIntensitySlider;
    juce::Slider drumIntensitySlider;

    // Preset selectors
    juce::ComboBox noisePresetCombo;
    juce::ComboBox drumPresetCombo;

    // Mix sliders
    juce::Slider noiseMixSlider;
    juce::Slider sidechainDuckSlider;
    juce::Slider drumMixSlider;
    juce::Slider humanizeSlider;

    // Global controls
    juce::ComboBox dropTypeCombo;
    juce::Slider outputLevelSlider;
    juce::Slider dryWetSlider;

    // Labels
    juce::Label titleLabel;
    juce::Label noiseLabel;
    juce::Label drumLabel;
    juce::Label noiseMixLabel;
    juce::Label sidechainLabel;
    juce::Label drumMixLabel;
    juce::Label humanizeLabel;
    juce::Label dropLabel;
    juce::Label outputLabel;
    juce::Label dryWetLabel;
    juce::Label progressLabel;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseIntensityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> drumIntensityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sidechainDuckAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> drumMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> humanizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoRetriggerAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> triggerAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> noisePresetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> drumPresetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dropTypeAttachment;

    // Helper methods
    void setupRotarySlider(juce::Slider& slider);
    void setupLinearSlider(juce::Slider& slider);
    void setupLabel(juce::Label& label, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BuildupFXAudioProcessorEditor)
};
