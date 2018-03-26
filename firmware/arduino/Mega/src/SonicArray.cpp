#include "SonicArray.h"
#include <Arduino.h>


SonicArray::SonicArray(unsigned int triggerPin)
{
    triggerPin_ = triggerPin;
}

void SonicArray::update()
{
    if (isTimeToMeasure())
    {
        // Disable the interrupts so we can prepare to calculate
        // the distances and then we are ready to start a new
        // set of measurements.
        noInterrupts(); // Begin critical section
        for (int i = 0; i < NUM_OF_SENSORS; i++)
        {
            // Copy the (volatile) start and end times of the pulses
            // so that we can begin immediately measuring again,
            // while distances of the (now previous) measurements are
            // being calculated.
            sensors_[i].prepareToCalculate();
            // Then reset previous the echoes so we can
            // conduct new measurements
            sensors_[i].reset();
        }
        // Finally start a new set of measurements
        // by triggering all sensors at once
        triggerSensors();
        interrupts(); // End critical section

        // Now that we are certain that our measurements are consistent
        // time-wise, calculate the distance.
        for (int i = 0; i < NUM_OF_SENSORS; i++)
        {
            // Calculate distance for each sensor.
            // Will also timeout any pending measurements
            sensors_[i].calculateDistance();
        }
    }
}

void SonicArray::triggerSensors()
{
    digitalWrite(triggerPin_, LOW);
    delayMicroseconds(3);
    digitalWrite(triggerPin_, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin_, LOW);
}

bool SonicArray::isTimeToMeasure()
{
    unsigned long currentTime = millis();
    if (currentTime - previousMeasurement_ >= measuramentInterval_)
    {
        previousMeasurement_ = currentTime;
        return true;
    }
    return false;
}
