#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BuildupFXAudioProcessorEditor::BuildupFXAudioProcessorEditor(BuildupFXAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set look and feel
    setLookAndFeel(&customLookAndFeel);

    // Set size
    setSize(700, 500);

    // Setup title
    titleLabel.setText("BuildupFX", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(28.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);

    // Setup buildup length buttons
    addAndMakeVisible(length4Button);
    addAndMakeVisible(length8Button);
    addAndMakeVisible(length16Button);
    addAndMakeVisible(length32Button);

    length4Button.setRadioGroupId(1);
    length8Button.setRadioGroupId(1);
    length16Button.setRadioGroupId(1);
    length32Button.setRadioGroupId(1);

    // Set default selection (16 bars)
    length16Button.setToggleState(true, juce::dontSendNotification);

    // Connect to parameter
    length4Button.onClick = [this] {
        if (length4Button.getToggleState())
            audioProcessor.parameters.getParameter("buildupLength")->setValueNotifyingHost(0.0f);
    };
    length8Button.onClick = [this] {
        if (length8Button.getToggleState())
            audioProcessor.parameters.getParameter("buildupLength")->setValueNotifyingHost(0.333f);
    };
    length16Button.onClick = [this] {
        if (length16Button.getToggleState())
            audioProcessor.parameters.getParameter("buildupLength")->setValueNotifyingHost(0.666f);
    };
    length32Button.onClick = [this] {
        if (length32Button.getToggleState())
            audioProcessor.parameters.getParameter("buildupLength")->setValueNotifyingHost(1.0f);
    };

    // Setup auto-retrigger toggle
    addAndMakeVisible(autoRetriggerButton);
    autoRetriggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "autoRetrigger", autoRetriggerButton);

    // Setup trigger button
    addAndMakeVisible(triggerButton);
    triggerButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffff4444));
    triggerButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    triggerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.parameters, "triggerButton", triggerButton);

    // Setup intensity sliders
    setupRotarySlider(noiseIntensitySlider);
    setupRotarySlider(drumIntensitySlider);

    noiseIntensityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "noiseIntensity", noiseIntensitySlider);
    drumIntensityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "drumIntensity", drumIntensitySlider);

    // Setup labels
    setupLabel(noiseLabel, "NOISE / FX");
    setupLabel(drumLabel, "DRUMS");

    // Setup preset combos
    addAndMakeVisible(noisePresetCombo);
    noisePresetCombo.addItem("Riser", 1);
    noisePresetCombo.addItem("Dark Tension", 2);
    noisePresetCombo.addItem("Bright Energy", 3);
    noisePresetCombo.addItem("Filtered", 4);
    noisePresetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.parameters, "noisePreset", noisePresetCombo);

    addAndMakeVisible(drumPresetCombo);
    drumPresetCombo.addItem("Snare Roll", 1);
    drumPresetCombo.addItem("Full Kit", 2);
    drumPresetCombo.addItem("Minimal", 3);
    drumPresetCombo.addItem("Kick Build", 4);
    drumPresetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.parameters, "drumPreset", drumPresetCombo);

    // Setup mix sliders
    setupLinearSlider(noiseMixSlider);
    setupLinearSlider(sidechainDuckSlider);
    setupLinearSlider(drumMixSlider);
    setupLinearSlider(humanizeSlider);

    noiseMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "noiseMix", noiseMixSlider);
    sidechainDuckAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "sidechainDuck", sidechainDuckSlider);
    drumMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "drumMix", drumMixSlider);
    humanizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "humanize", humanizeSlider);

    setupLabel(noiseMixLabel, "Mix:");
    setupLabel(sidechainLabel, "Duck:");
    setupLabel(drumMixLabel, "Mix:");
    setupLabel(humanizeLabel, "Human:");

    // Setup drop type combo
    addAndMakeVisible(dropTypeCombo);
    dropTypeCombo.addItem("None", 1);
    dropTypeCombo.addItem("Vinyl Brake", 2);
    dropTypeCombo.addItem("Filter Drop", 3);
    dropTypeCombo.addItem("Impact", 4);
    dropTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.parameters, "dropType", dropTypeCombo);
    setupLabel(dropLabel, "Drop:");

    // Setup output controls
    setupLinearSlider(outputLevelSlider);
    outputLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "outputLevel", outputLevelSlider);
    setupLabel(outputLabel, "Output:");

    setupLinearSlider(dryWetSlider);
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "dryWet", dryWetSlider);
    setupLabel(dryWetLabel, "Dry/Wet:");

    // Setup progress label
    setupLabel(progressLabel, "Ready");
    progressLabel.setFont(juce::Font(16.0f, juce::Font::bold));

    // Start timer for UI updates (30 fps)
    startTimer(33);
}

BuildupFXAudioProcessorEditor::~BuildupFXAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    stopTimer();
}

//==============================================================================
void BuildupFXAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(0xff1a1a1a));

    // Header bar
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRect(0, 0, getWidth(), 60);

    // Section boxes
    auto noiseSection = juce::Rectangle<int>(20, 120, 320, 240);
    auto drumSection = juce::Rectangle<int>(360, 120, 320, 240);

    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRoundedRectangle(noiseSection.toFloat(), 8.0f);
    g.fillRoundedRectangle(drumSection.toFloat(), 8.0f);

    g.setColour(juce::Colour(0xff444444));
    g.drawRoundedRectangle(noiseSection.toFloat(), 8.0f, 2.0f);
    g.drawRoundedRectangle(drumSection.toFloat(), 8.0f, 2.0f);

    // Progress bar background
    auto progressBounds = juce::Rectangle<int>(20, 380, 660, 30);
    g.setColour(juce::Colour(0xff333333));
    g.fillRoundedRectangle(progressBounds.toFloat(), 4.0f);

    // Progress bar fill
    float progress = audioProcessor.getBuildupProgress();
    if (progress > 0.0f)
    {
        auto fillWidth = progressBounds.getWidth() * progress;
        auto fillBounds = progressBounds.withWidth(static_cast<int>(fillWidth));

        // Color based on progress
        juce::Colour fillColour;
        if (progress < 0.3f)
            fillColour = juce::Colour(0xff00ff00);
        else if (progress < 0.7f)
            fillColour = juce::Colour(0xff00ff00).interpolatedWith(juce::Colour(0xffffff00), (progress - 0.3f) / 0.4f);
        else
            fillColour = juce::Colour(0xffffff00).interpolatedWith(juce::Colour(0xffff0000), (progress - 0.7f) / 0.3f);

        g.setColour(fillColour);
        g.fillRoundedRectangle(fillBounds.toFloat(), 4.0f);
    }

    // "LENGTH:" label
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("LENGTH:", 20, 70, 70, 20, juce::Justification::centredLeft);
    g.drawText("Bars", 280, 70, 50, 20, juce::Justification::centredLeft);
}

void BuildupFXAudioProcessorEditor::resized()
{
    // Title
    titleLabel.setBounds(0, 10, getWidth(), 40);

    // Buildup length buttons
    length4Button.setBounds(90, 70, 40, 20);
    length8Button.setBounds(135, 70, 40, 20);
    length16Button.setBounds(180, 70, 40, 20);
    length32Button.setBounds(225, 70, 40, 20);

    // Auto-retrigger and trigger
    autoRetriggerButton.setBounds(340, 70, 150, 25);
    triggerButton.setBounds(520, 60, 160, 40);

    // Noise section (left)
    int noiseSectionX = 30;
    int noiseSectionY = 130;

    noiseLabel.setBounds(noiseSectionX, noiseSectionY - 5, 300, 20);
    noiseIntensitySlider.setBounds(noiseSectionX + 80, noiseSectionY + 20, 140, 140);

    noisePresetCombo.setBounds(noiseSectionX + 15, noiseSectionY + 170, 280, 25);
    noiseMixLabel.setBounds(noiseSectionX + 15, noiseSectionY + 200, 40, 20);
    noiseMixSlider.setBounds(noiseSectionX + 60, noiseSectionY + 200, 235, 20);
    sidechainLabel.setBounds(noiseSectionX + 15, noiseSectionY + 225, 40, 20);
    sidechainDuckSlider.setBounds(noiseSectionX + 60, noiseSectionY + 225, 235, 20);

    // Drum section (right)
    int drumSectionX = 370;
    int drumSectionY = 130;

    drumLabel.setBounds(drumSectionX, drumSectionY - 5, 300, 20);
    drumIntensitySlider.setBounds(drumSectionX + 80, drumSectionY + 20, 140, 140);

    drumPresetCombo.setBounds(drumSectionX + 15, drumSectionY + 170, 280, 25);
    drumMixLabel.setBounds(drumSectionX + 15, drumSectionY + 200, 40, 20);
    drumMixSlider.setBounds(drumSectionX + 60, drumSectionY + 200, 235, 20);
    humanizeLabel.setBounds(drumSectionX + 15, drumSectionY + 225, 50, 20);
    humanizeSlider.setBounds(drumSectionX + 70, drumSectionY + 225, 225, 20);

    // Progress label
    progressLabel.setBounds(20, 420, 660, 30);

    // Global controls (bottom)
    dropLabel.setBounds(20, 460, 50, 20);
    dropTypeCombo.setBounds(70, 460, 150, 25);

    outputLabel.setBounds(380, 460, 60, 20);
    outputLevelSlider.setBounds(440, 460, 120, 20);

    dryWetLabel.setBounds(380, 485, 60, 20);
    dryWetSlider.setBounds(440, 485, 120, 20);
}

void BuildupFXAudioProcessorEditor::timerCallback()
{
    // Update progress label
    auto state = audioProcessor.getCurrentState();

    if (state == BuildupFXAudioProcessor::BuildupState::BUILDING)
    {
        int barsRemaining = audioProcessor.getBarsRemaining();
        progressLabel.setText(juce::String("Building... ") + juce::String(barsRemaining) + " bars left",
                             juce::dontSendNotification);
    }
    else if (state == BuildupFXAudioProcessor::BuildupState::DROPPING)
    {
        progressLabel.setText("DROP!", juce::dontSendNotification);
    }
    else
    {
        progressLabel.setText("Ready", juce::dontSendNotification);
    }

    // Repaint for progress bar
    repaint();
}

//==============================================================================
void BuildupFXAudioProcessorEditor::setupRotarySlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                               juce::MathConstants<float>::pi * 2.75f,
                               true);
    addAndMakeVisible(slider);
}

void BuildupFXAudioProcessorEditor::setupLinearSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(slider);
}

void BuildupFXAudioProcessorEditor::setupLabel(juce::Label& label, const juce::String& text)
{
    label.setText(text, juce::dontSendNotification);
    label.setFont(juce::Font(12.0f));
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    label.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(label);
}
