/*
  ==============================================================================

    RotaryKnob.cpp
    Created: 20 Aug 2024 12:26:26pm
    Author:  Riley Ham

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID, bool drawFromMiddle)
    : attachment(apvts, parameterID.getParamID(), slider)
{
    // make the start and end angkes of the knob a bit less extreme
    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.25f * pi, 2.75f * pi, true);

    // set slider style elements
    slider.getProperties().set("drawFromMiddle", drawFromMiddle);
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    slider.setBounds(0, 0, 70, 86);
    addAndMakeVisible(slider);
    
    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>(0, 0, 2, 0));     // puts 2 pixels of space between the bottom of the label and top of the knob
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
    
    setLookAndFeel(RotaryKnobLookAndFeel::get());

    
    setSize(70, 110);
}

RotaryKnob::~RotaryKnob()
{
}


void RotaryKnob::resized()
{
    slider.setTopLeftPosition(0, 24);
}
