#ifndef SONIC_ARRAY_H
#define SONIC_ARRAY_H

#include <SonicSensor.h>
#include <Arduino.h>

#define NUM_OF_SENSORS 7
#define MAX_DETECTION_DISTANCE 200
#define GRID_RESOLUTION_RATIO 10
#define GRID_SIZE_X MAX_DETECTION_DISTANCE / GRID_RESOLUTION_RATIO
#define GRID_SIZE_Y MAX_DETECTION_DISTANCE / GRID_RESOLUTION_RATIO

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
    float obstacleAvoidance();

  private:
    unsigned int triggerPin_;
    unsigned long previousMeasurement_, measuramentInterval_;
    void triggerSensors();
    bool isTimeToMeasure();

    SonicSensor sensors_[NUM_OF_SENSORS] = {
        SonicSensor(12, 0),  // Ultrasonic_0 on the Sonic Disc
        SonicSensor(A8, 0),  // Ultrasonic_1
        SonicSensor(A9, 0),  // Ultrasonic_2
        SonicSensor(A10, 0), // Ultrasonic_3
        SonicSensor(A11, 0), // Ultrasonic_4
        SonicSensor(A12, 0), // Ultrasonic_5
        SonicSensor(A13, 0), // Ultrasonic_6
        //SonicSensor(A14)  // Ultrasonic_7
        //SonicSensor(A15)  // Ultrasonic_8
    };

    bool obstacleGrid_[GRID_SIZE_X][GRID_SIZE_Y];
};

#endif //SONIC_ARRAY_H