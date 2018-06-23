#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

inline float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline float lerp(float a, float b, float t)
{
    return (1-t)*a + t*b;
}

inline void setupChangeInterrupt(unsigned char pin)
{
    pinMode(pin, INPUT_PULLUP);
    // Enable interrupt for pin
    *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));
    // Clear any outstanding interrupt
    PCIFR |= bit(digitalPinToPCICRbit(pin));
    // Enable interrupt for the pin's group
    PCICR |= bit(digitalPinToPCICRbit(pin));
}

#endif
