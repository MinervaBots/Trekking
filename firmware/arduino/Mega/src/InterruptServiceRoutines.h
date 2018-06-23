#ifndef INTERRUPT_SERVICE_ROUTINES_H
#define INTERRUPT_SERVICE_ROUTINES_H

#include <Arduino.h>
#include "Variables.h"
#include "RCReceiver.h"

bool lastGearState = LOW;
ISR(PCINT3_vect)
{
    bool currentGearState = digitalRead(GEAR_PIN);
    Serial.print("currentGearState: \t");
    Serial.println(currentGearState);
    Serial.print("lastGearState: \t");
    Serial.println(lastGearState);
    if(lastGearState != currentGearState)
    {
        lastGearState = currentGearState;
        gearISR();
    }
}

ISR(PCINT0_vect)
{
    sonicArray.handleEcho(SonicArray::Vector::VECTOR_0);
}

ISR(PCINT2_vect)
{
    sonicArray.handleEcho(SonicArray::Vector::VECTOR_2);
}

#endif // INTERRUPT_SERVICE_ROUTINES_H