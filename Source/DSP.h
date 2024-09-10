/*
  ==============================================================================

    DSP.h
    Created: 9 Sep 2024 10:53:01pm
    Author:  Riley Ham

  ==============================================================================
*/

#pragma once

#include <cmath>

inline void panningEqualPower(float panning, float& left, float& right)
{
            // pi / 4
    float x = 0.7853981633974483f * (panning + 1.0f);
    left = std::cos(x);
    right = std::sin(x);
}
