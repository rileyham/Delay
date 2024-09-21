/*
  ==============================================================================

    Measurement.h
    Created: 21 Sep 2024 5:17:08pm
    Author:  Riley Ham

  ==============================================================================
*/

#pragma once

#include <atomic>

struct Measurement
{
    void reset() noexcept
    {
        value.store(0.0f);
    }
    
    void updateIfGreater(float newValue) noexcept
    {
        auto oldValue = value.load();
        while (newValue > oldValue && !value.compare_exchange_weak(oldValue, newValue));
    }
    
    float readAndReset() noexcept
    {
        return value.exchange(0.0f);
    }
    
    std::atomic<float> value;
};
