/*
  ==============================================================================

    RotaryKnob.h
    Created: 20 Aug 2024 12:26:26pm
    Author:  Riley Ham

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RotaryKnob  : public juce::Component
{
public:
    RotaryKnob(const juce::String& text, juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& parameterID);
    ~RotaryKnob() override;

    void resized() override;
    
    juce::Slider slider;
    juce::Label label;
    
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

private:

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
