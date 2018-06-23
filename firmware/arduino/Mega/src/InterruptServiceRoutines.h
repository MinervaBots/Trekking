#ifndef INTERRUPT_SERVICE_ROUTINES_H
#define INTERRUPT_SERVICE_ROUTINES_H

#include <Arduino.h>
#include "Variables.h"
#include "RCReceiver.h"


bool lastGearState = LOW;
ISR(PCINT0_vect)
{
    bool currentGearState = digitalRead(GEAR_PIN);
    if(lastGearState != currentGearState)
    {
        lastGearState = currentGearState;
        gearISR();
    }
    sonicArray.handleEcho(SonicArray::Vector::VECTOR_0);
}

ISR(PCINT2_vect)
{
    sonicArray.handleEcho(SonicArray::Vector::VECTOR_2);
}

#endif // INTERRUPT_SERVICE_ROUTINES_H