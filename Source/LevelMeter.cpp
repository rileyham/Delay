/*
  ==============================================================================

    LevelMeter.cpp
    Created: 20 Sep 2024 11:51:18am
    Author:  Riley Ham

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LevelMeter.h"
#include "LookAndFeel.h"

//==============================================================================
LevelMeter::LevelMeter(Measurement& measurementL_, Measurement& measurementR_) : measurementL(measurementL_), measurementR(measurementR_),
dbLevelL(clampDb), dbLevelR(clampDb)
{
    setOpaque(true);
    startTimerHz(refreshRate);
    
    // calculate the coefficient for the one pole smoothing filter
    decay = 1.0f - std::exp(-1.0f / (float(refreshRate) * 0.2f));
}

LevelMeter::~LevelMeter()
{
}

void LevelMeter::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    
    g.fillAll(Colors::LevelMeter::background);
    
    // draw level rects
    drawLevel(g, dbLevelL, 0, 7);
    drawLevel(g, dbLevelR, 9, 7);
    
    g.setFont(Fonts::getFont(10.0f));
    for (float db = maxDb; db >= minDb; db -= stepDb) {
        int y = positionForLevel(db);
        
        g.setColour(Colors::LevelMeter::tickLine);
        g.fillRect(0, y, 16, 1);
        
        g.setColour(Colors::LevelMeter::tickLabel);
        g.drawSingleLineText(juce::String(int(db)), bounds.getWidth(), y + 3, juce::Justification::right);
    }

}

void LevelMeter::resized()
{
    maxPos = 4.0f;
    minPos = float(getHeight()) - 4.0f;
}

void LevelMeter::timerCallback()
{
    updateLevel(measurementL.readAndReset(), levelL, dbLevelL);
    updateLevel(measurementR.readAndReset(), levelR, dbLevelR);
    
    repaint();
}

void LevelMeter::drawLevel(juce::Graphics &g, float level, int x, int width)
{
    int y = positionForLevel(level);
    if (level > 0.0f) {  // if level is peaking
        int y0 = positionForLevel(0.0f);
        g.setColour(Colors::LevelMeter::tooLoud);
        g.fillRect(x, y, width, y0 - y);
        g.setColour(Colors::LevelMeter::levelOK);
        g.fillRect(x, y0, width, getHeight() - y);
    }
    else if (y < getHeight()) {
        g.setColour(Colors::LevelMeter::levelOK);
        g.fillRect(x, y, width, getHeight() - y);
    }
}

void LevelMeter::updateLevel(float newLevel, float &smoothedLevel, float &levelDb) const
{
    // applies the animation
    if (newLevel > smoothedLevel) {
        smoothedLevel = newLevel; // instantaneous attack
    }
    else {
        smoothedLevel += (newLevel - smoothedLevel) * decay; // smooth decay
    }
    
    if (smoothedLevel > clampLevel) {
        levelDb = juce::Decibels::gainToDecibels(smoothedLevel);
    }
    else {
        levelDb = clampDb;
    }
}
