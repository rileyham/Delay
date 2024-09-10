/*
  ==============================================================================

    Parameters.cpp
    Created: 24 Jul 2024 1:27:28pm
    Author:  Riley Ham

  ==============================================================================
*/

#include "Parameters.h"
#include "DSP.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
}

static juce::String stringFromMilliseconds(float value, int)
{
    if (value < 10.0f) {
        return juce::String(value, 2) + " ms";
    }
    if (value < 100.0f) {
        return juce::String(value, 1) + " ms";
    }
    if (value < 1000.0f) {
        return juce::String(int(value)) + " ms";
    }
    else {
        return juce::String(value * 0.001f, 2 ) + " s";
    }
}

static float millisecondsFromString(const juce::String& text)
{
    float value = text.getFloatValue();
    if(!text.endsWithIgnoreCase("ms")) {
        if(text.endsWithIgnoreCase("s") || value < Parameters::MIN_DELAY_TIME) {
            return value * 1000.0f;
        }
    }
    return value;
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + " db";
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value))+ " %";
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, delayTimeParamID, delayTimeParam);
    castParameter(apvts, mixParamID, mixParam);
    castParameter(apvts, feedbackParamID, feedbackParam);
    castParameter(apvts, stereoParamID, stereoParam);
}


juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(gainParamID,
                                                           "Output Gain",
                                                           juce::NormalisableRange<float> {-12.0f, 12.0f},
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                    .withStringFromValueFunction(stringFromDecibels)
                                                           ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(delayTimeParamID,
                                                           "Delay Time",
                                                           // 0.001f is the step size, 0.3 is the skew factor (the smaller the number, the more emphasis low values recieve
                                                           juce::NormalisableRange<float> {MIN_DELAY_TIME, MAX_DELAY_TIME, 0.001f, 0.3f},
                                                           100.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                    .withStringFromValueFunction(stringFromMilliseconds)
                                                                    .withValueFromStringFunction(millisecondsFromString)
                                                           ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(mixParamID,
                                                           "Mix",
                                                           juce::NormalisableRange<float> {0.0f, 100.0f, 1.0f},
                                                           100.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                    .withStringFromValueFunction(stringFromPercent)
                                                           ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(feedbackParamID,
                                                           "Feedback",
                                                           juce::NormalisableRange<float> {-100.0f, 100.0f, 1.0f},
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                    .withStringFromValueFunction(stringFromPercent)
                                                           ));
    layout.add(std::make_unique<juce::AudioParameterFloat>(stereoParamID,
                                                           "Stereo",
                                                           juce::NormalisableRange<float> {-100.0f, 100.0f, 1.0f},
                                                           0.0f,
                                                           juce::AudioParameterFloatAttributes()
                                                                    .withStringFromValueFunction(stringFromPercent)
                                                           ));
    
    
    
    return layout;
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    targetDelayTime = delayTimeParam->get();
    if (delayTime == 0.0f) {
        delayTime = targetDelayTime;
    }
    
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
    stereoSmoother.setTargetValue(stereoParam->get() * 0.01f);
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);
    feedbackSmoother.reset(sampleRate, duration);
    stereoSmoother.reset(sampleRate, duration);
    
    // calculate filter coefficient
    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    delayTime = 0.0f;
    mix = 1.0f;
    feedback = 0.0f;
    
    panL = 0.0f;
    panR = 1.0f;
    
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setCurrentAndTargetValue(feedbackParam->get() * 0.01f);
    stereoSmoother.setCurrentAndTargetValue(stereoParam->get() * 0.01f);

}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    delayTime += (targetDelayTime - delayTime) * coeff;
    mix =  mixSmoother.getNextValue();
    feedback = feedbackSmoother.getNextValue();
    panningEqualPower(stereoSmoother.getNextValue(), panL, panR);
}
