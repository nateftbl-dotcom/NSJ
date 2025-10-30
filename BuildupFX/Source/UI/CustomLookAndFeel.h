#pragma once
#include <JuceHeader.h>

/**
 * Custom look and feel for BuildupFX
 */
class BuildupLookAndFeel : public juce::LookAndFeel_V4
{
public:
    BuildupLookAndFeel()
    {
        // Set color scheme
        setColour(juce::Slider::thumbColourId, juce::Colour(0xff00bfff));
        setColour(juce::Slider::trackColourId, juce::Colour(0xff333333));
        setColour(juce::Slider::backgroundColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff00bfff));
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff333333));

        setColour(juce::TextButton::buttonColourId, juce::Colour(0xff444444));
        setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff4444));
        setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        setColour(juce::TextButton::textColourOnId, juce::Colours::white);

        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff2a2a2a));
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff444444));
        setColour(juce::ComboBox::textColourId, juce::Colours::white);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override
    {
        auto radius = juce::jmin(width / 2, height / 2) - 10.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // Get intensity color (green -> yellow -> red)
        juce::Colour fillColour;
        if (sliderPos < 0.3f)
            fillColour = juce::Colour(0xff00ff00); // Green
        else if (sliderPos < 0.7f)
            fillColour = juce::Colour(0xff00ff00).interpolatedWith(juce::Colour(0xffffff00), (sliderPos - 0.3f) / 0.4f);
        else
            fillColour = juce::Colour(0xffffff00).interpolatedWith(juce::Colour(0xffff0000), (sliderPos - 0.7f) / 0.3f);

        // Draw background arc
        g.setColour(juce::Colour(0xff333333));
        g.fillEllipse(rx, ry, rw, rw);

        // Draw value arc
        if (sliderPos > 0.0f)
        {
            juce::Path valueArc;
            valueArc.addCentredArc(centreX, centreY, radius - 5.0f, radius - 5.0f,
                                   0.0f, rotaryStartAngle, angle, true);

            g.setColour(fillColour);
            g.strokePath(valueArc, juce::PathStrokeType(5.0f));
        }

        // Draw pointer
        juce::Path pointer;
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 3.0f;
        pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(juce::Colours::white);
        g.fillPath(pointer);

        // Draw value in center
        g.setColour(juce::Colours::white);
        g.setFont(18.0f);
        auto valueText = juce::String(static_cast<int>(slider.getValue()));
        g.drawText(valueText, rx, ry + rw * 0.4f, rw, 20, juce::Justification::centred);
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (slider.isHorizontal())
        {
            auto trackHeight = 4.0f;
            auto trackY = y + (height - trackHeight) * 0.5f;

            // Draw track background
            g.setColour(juce::Colour(0xff333333));
            g.fillRoundedRectangle(x, trackY, width, trackHeight, 2.0f);

            // Draw filled track
            g.setColour(juce::Colour(0xff00bfff));
            g.fillRoundedRectangle(x, trackY, sliderPos - x, trackHeight, 2.0f);

            // Draw thumb
            auto thumbWidth = 12.0f;
            auto thumbHeight = 16.0f;
            auto thumbX = sliderPos - thumbWidth * 0.5f;
            auto thumbY = trackY - (thumbHeight - trackHeight) * 0.5f;

            g.setColour(juce::Colours::white);
            g.fillRoundedRectangle(thumbX, thumbY, thumbWidth, thumbHeight, 2.0f);
        }
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(2.0f);

        // Draw checkbox
        g.setColour(button.getToggleState() ? juce::Colour(0xff00bfff) : juce::Colour(0xff333333));
        g.fillRoundedRectangle(bounds.removeFromLeft(20.0f), 2.0f);

        // Draw checkmark if toggled
        if (button.getToggleState())
        {
            g.setColour(juce::Colours::white);
            juce::Path tick;
            tick.startNewSubPath(6.0f, 10.0f);
            tick.lineTo(10.0f, 14.0f);
            tick.lineTo(16.0f, 6.0f);
            g.strokePath(tick, juce::PathStrokeType(2.0f));
        }

        // Draw label
        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        g.drawText(button.getButtonText(), bounds, juce::Justification::centredLeft);
    }
};
