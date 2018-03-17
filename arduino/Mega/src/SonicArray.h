#ifndef SONIC_ARRAY_H
#define SONIC_ARRAY_H

#include <SonicSensor.h>
#include <Arduino.h>

#define NUM_OF_SENSORS 8

class SonicArray
{
public:
    SonicArray(unsigned int triggerPin);
    void update();

private:
    unsigned int triggerPin_;
    unsigned long previousMeasurement_, measuramentInterval_;
    void triggerSensors();
    bool isTimeToMeasure();

    SonicSensor sensors_[NUM_OF_SENSORS] = {
    SonicSensor(A2),  // Ultrasonic_0 on the Sonic Disc
    SonicSensor(A0),  // Ultrasonic_1
    SonicSensor(12),  // Ultrasonic_2
    SonicSensor(10),  // Ultrasonic_3
    SonicSensor(9),    // Ultrasonic_4
    SonicSensor(6),    // Ultrasonic_5
    SonicSensor(5),    // Ultrasonic_6
    SonicSensor(2)     // Ultrasonic_7
    };
};


#endif //SONIC_ARRAY_H