/*
  ==============================================================================

    LevelMeter.h
    Created: 20 Sep 2024 11:51:18am
    Author:  Riley Ham

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Measurement.h"

//==============================================================================
/*
*/
class LevelMeter  : public juce::Component, private juce::Timer
{
public:
    LevelMeter(Measurement& measurementL, Measurement& measurementR);
    ~LevelMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void drawLevel(juce::Graphics& g, float level, int x, int width);
    void updateLevel(float newLevel, float& smoothedLevel, float& levelDb) const;
    int positionForLevel(float dbLevel) const noexcept
    {
        return int(std::round(juce::jmap(dbLevel, maxDb, minDb, maxPos, minPos)));
    }
    
    Measurement& measurementL;
    Measurement& measurementR;
    
    // drawing variables
    static constexpr float maxDb = 6.0f;
    static constexpr float minDb = -60.0f;
    static constexpr float stepDb = 6.0f;
    
    float maxPos = 0.0f;
    float minPos = 0.0f;
    
    static constexpr float clampDb = -120.0f;
    static constexpr float clampLevel = 0.000001f; // -120 db
    
    float dbLevelL;
    float dbLevelR;
    
    // smooth level decay
    static constexpr int refreshRate = 60;
    
    float decay = 0.0f;
    float levelL = clampLevel;
    float levelR = clampLevel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};
