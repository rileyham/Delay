/*
  ==============================================================================

    Parameters.h
    Created: 24 Jul 2024 1:27:28pm
    Author:  Henry Ham

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID {"gain", 1};
const juce::ParameterID delayTimeParamID {"delayTime", 1};
const juce::ParameterID mixParamID {"mix", 1};
const juce::ParameterID feedbackParamID {"feedback", 1};
const juce::ParameterID stereoParamID {"stereo", 1};



class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    
    // plugin parameter values
    float gain = 0.0f;
    float delayTime = 0.0f;
    float mix = 1.0f;
    float feedback = 0.0f;
    float panL = 0.0f;
    float panR = 1.0f;
    
    float targetDelayTime = 0.0f;
    float coeff = 0.0f; // one-pole smoothing
    
    static constexpr float MIN_DELAY_TIME = 5.0f;
    static constexpr float MAX_DELAY_TIME = 5000.0f;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Parameters)
    
    // parameter objects
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
    
    juce::AudioParameterFloat* delayTimeParam;
    
    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;
    
    juce::AudioParameterFloat* feedbackParam;
    juce::LinearSmoothedValue<float> feedbackSmoother;
    
    juce::AudioParameterFloat* stereoParam;
    juce::LinearSmoothedValue<float> stereoSmoother;
};
