#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// StepButton Implementation
//==============================================================================

StepButton::StepButton(int index)
    : Button("Step" + juce::String(index))
    , lockState(SimpleSequencer::UNLOCKED)
    , isActive(false)
    , isCurrent(false)
    , stepIndex(index)
{
    setClickingTogglesState(false);
}

void StepButton::setLockState(SimpleSequencer::GateState state)
{
    lockState = state;
    repaint();
}

void StepButton::setActive(bool active)
{
    isActive = active;
    repaint();
}

void StepButton::setCurrentStep(bool current)
{
    isCurrent = current;
    repaint();
}

void StepButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, 
                              bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsDown);
    
    auto bounds = getLocalBounds().toFloat();
    
    // Background color based on active state
    juce::Colour bgColour;
    if (isActive)
        bgColour = juce::Colour(0xff4080ff);  // Blue when active
    else
        bgColour = juce::Colour(0xff2a2a2a);  // Dark gray when inactive
    
    // Lighten on hover
    if (shouldDrawButtonAsHighlighted)
        bgColour = bgColour.brighter(0.2f);
    
    g.setColour(bgColour);
    g.fillRoundedRectangle(bounds.reduced(2.0f), 4.0f);
    
    // Border based on lock state
    juce::Colour borderColour;
    float borderThickness = 2.0f;
    
    switch (lockState)
    {
        case SimpleSequencer::LOCKED_ON:
            borderColour = juce::Colours::green;
            borderThickness = 3.0f;
            break;
        case SimpleSequencer::LOCKED_OFF:
            borderColour = juce::Colours::red;
            borderThickness = 3.0f;
            break;
        case SimpleSequencer::UNLOCKED:
        default:
            borderColour = juce::Colour(0xff404040);
            break;
    }
    
    g.setColour(borderColour);
    g.drawRoundedRectangle(bounds.reduced(2.0f), 4.0f, borderThickness);
    
    // Highlight current step
    if (isCurrent)
    {
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        g.drawRoundedRectangle(bounds.reduced(1.0f), 4.0f, 2.0f);
    }
    
    // Draw step number
    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.setFont(10.0f);
    g.drawText(juce::String(stepIndex + 1), bounds, juce::Justification::centred);
}

//==============================================================================
// Editor Implementation
//==============================================================================

NullSpaceJeneratorAudioProcessorEditor::NullSpaceJeneratorAudioProcessorEditor(
    NullSpaceJeneratorAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
{
    // Set window size
    setSize(800, 240);
    
    // Title label
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Null Space Jenerator", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    // Create step buttons
    for (int i = 0; i < SimpleSequencer::NUM_STEPS; ++i)
    {
        stepButtons[i] = std::make_unique<StepButton>(i);
        addAndMakeVisible(stepButtons[i].get());
        
        stepButtons[i]->onClick = [this, i]()
        {
            stepButtonClicked(i, juce::ModifierKeys::getCurrentModifiers());
        };
    }
    
    // Regenerate button
    addAndMakeVisible(regenerateButton);
    regenerateButton.setButtonText("REGENERATE");
    regenerateButton.onClick = [this]() { regenerateButtonClicked(); };
    regenerateButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff404040));
    regenerateButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    
    // Start timer for UI updates
    startTimerHz(30);  // 30 FPS
}

NullSpaceJeneratorAudioProcessorEditor::~NullSpaceJeneratorAudioProcessorEditor()
{
}

void NullSpaceJeneratorAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Dark background
    g.fillAll(juce::Colour(0xff1a1a1a));
}

void NullSpaceJeneratorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    
    // Title at top
    titleLabel.setBounds(area.removeFromTop(40).reduced(10, 5));
    
    area.removeFromTop(10);  // Spacing
    
    // Step grid in center
    auto gridArea = area.removeFromTop(80).reduced(20, 0);
    int stepWidth = gridArea.getWidth() / SimpleSequencer::NUM_STEPS;
    
    for (int i = 0; i < SimpleSequencer::NUM_STEPS; ++i)
    {
        stepButtons[i]->setBounds(gridArea.getX() + i * stepWidth + 2, 
                                  gridArea.getY(), 
                                  stepWidth - 4, 
                                  gridArea.getHeight());
    }
    
    area.removeFromTop(20);  // Spacing
    
    // Regenerate button at bottom
    regenerateButton.setBounds(area.reduced(200, 10));
}

void NullSpaceJeneratorAudioProcessorEditor::timerCallback()
{
    // Update step button states
    auto& sequencer = audioProcessor.getSequencer();
    int currentStep = sequencer.getCurrentStep();
    
    for (int i = 0; i < SimpleSequencer::NUM_STEPS; ++i)
    {
        stepButtons[i]->setLockState(sequencer.getGateState(i));
        stepButtons[i]->setActive(sequencer.isGateActive(i));
        stepButtons[i]->setCurrentStep(i == currentStep);
    }
}

void NullSpaceJeneratorAudioProcessorEditor::stepButtonClicked(int stepIndex, 
                                                                const juce::ModifierKeys& mods)
{
    auto& sequencer = audioProcessor.getSequencer();
    
    if (mods.isRightButtonDown() || mods.isCommandDown())
    {
        // Right-click or Cmd+Click: toggle lock state
        sequencer.toggleLock(stepIndex);
    }
    else
    {
        // Left-click: toggle gate on/off
        sequencer.toggleGate(stepIndex);
    }
}

void NullSpaceJeneratorAudioProcessorEditor::regenerateButtonClicked()
{
    audioProcessor.getSequencer().regenerate();
}
