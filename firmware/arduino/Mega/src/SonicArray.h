#ifndef SONIC_ARRAY_H
#define SONIC_ARRAY_H

#include <SonicSensor.h>
#include <Arduino.h>

#define NUM_OF_SENSORS 8

class SonicArray
{
  public:
    enum Vector
    {
        VECTOR_0, // PCINT0_vect
        VECTOR_2  // PCINT2_vect
    };

    SonicArray(unsigned int triggerPin);
    void update();
    void handleEcho(Vector intVector);
    void setupChangeInterrupt();

  private:
    unsigned int triggerPin_;
    unsigned long previousMeasurement_, measuramentInterval_;
    void triggerSensors();
    bool isTimeToMeasure();

    SonicSensor sensors_[NUM_OF_SENSORS] = {
        SonicSensor(12),  // Ultrasonic_0 on the Sonic Disc
        SonicSensor(A8),  // Ultrasonic_1
        SonicSensor(A9),  // Ultrasonic_2
        SonicSensor(A10), // Ultrasonic_3
        SonicSensor(A11), // Ultrasonic_4
        SonicSensor(A12), // Ultrasonic_5
        SonicSensor(A13), // Ultrasonic_6
        SonicSensor(A14)  // Ultrasonic_7
        //SonicSensor(A15)  // Ultrasonic_8
    };
};

#endif //SONIC_ARRAY_H