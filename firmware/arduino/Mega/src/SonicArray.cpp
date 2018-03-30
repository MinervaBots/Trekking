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

/**
 * Handles the echo signal from an ultrasonic sensor.
 * To be called from within an interrupt, it checks all the echo
 * pins to see if any of them is HIGH or LOW and updates the echo
 * pulse times of the respective sensor.
 * If a pulse has already been set, then the new value is ignored.
 * @param intVector The vector that the interrupt originated from
 */
void SonicArray::handleEcho(Vector intVector)
{
    // Determine which vector the interrupt originated from
    // so we only check signals from those specific sensors
    int sensorsInVector[7] = {0}; // We have up to 7 sensors in each vector
    switch (intVector)
    {
    case VECTOR_0:
        // Ultrasonic 0 is on PCINT0_vect
        sensorsInVector[0] = 0;
        sensorsInVector[1] = 0;
        sensorsInVector[2] = 0;
        sensorsInVector[3] = 0;
        sensorsInVector[4] = 0;
        sensorsInVector[5] = 0;
        sensorsInVector[6] = 0;
        break;

    case VECTOR_2:
        // Ultrasonics 1, 2, 3, 4, 5, 6, 7 and 8 are on PCINT2_vect
        sensorsInVector[0] = 1;
        sensorsInVector[1] = 2;
        sensorsInVector[2] = 3;
        sensorsInVector[3] = 4;
        sensorsInVector[4] = 5;
        sensorsInVector[5] = 6;
        sensorsInVector[6] = 7;
        // sensorsInVector[7] = 8;
        break;
    default:
        break; // We should not be here
    }

    // Iterate through the specific vector's ultrasonic echo pins
    for (int i : sensorsInVector)
    {
        // If a pin is HIGH, it means that a pulse
        // is either just starting or has previously started.
        // We only care about the former.
        if (digitalRead(sensors_[i].getEchoPin()) == HIGH)
        {
            // We only care for newly generated pulses and not ones
            // we have handled before.
            if (sensors_[i].getStartOfPulse() == 0)
            {
                sensors_[i].setStartOfPulse(micros());
            }
        }
        else
        {
            // If a pin is LOW, it means that a pulse has just ended,
            // has already ended or not started. We only care about
            // the first case. We can determine this by pulses which
            // we have not handled before AND that have already started
            if (sensors_[i].getEndOfPulse() == 0 && sensors_[i].getStartOfPulse() != 0)
            {
                sensors_[i].setEndOfPulse(micros());
            }
        }
    }
}

void SonicArray::setupChangeInterrupt()
{
    for (int i = 0; i < NUM_OF_SENSORS; i++)
    {
        auto echoPin = sensors_[i].getEchoPin();
        pinMode(echoPin, INPUT);
        // Enable interrupt for pin
        *digitalPinToPCMSK(echoPin) |= bit(digitalPinToPCMSKbit(echoPin));
        // Clear any outstanding interrupt
        PCIFR |= bit(digitalPinToPCICRbit(echoPin));
        // Enable interrupt for the pin's group
        PCICR |= bit(digitalPinToPCICRbit(echoPin));
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
