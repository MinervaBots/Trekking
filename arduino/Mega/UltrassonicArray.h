#ifndef ULTRASSONIC_ARRAY_H
#define ULTRASSONIC_ARRAY_H

#include <Arduino.h>
#include "Constants.h"

struct SensorResult
{
  float distance, direction;
};
struct SensorConfig
{
  unsigned char trigger, echo;
  float direction;
  
  SensorConfig(unsigned char trigger, unsigned char echo, float direction)
  {
    this->trigger = trigger;
    this->echo = echo;
    this->direction = direction;

    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
  }
  
  SensorConfig()
  {
    
  }
};

class UltrassonicArray
{
public:
  UltrassonicArray();
  bool add(float direction, unsigned char trigger, unsigned char echo);
  SensorResult evaluate(int sensorId);

private:
  unsigned int sensorCount_;
  SensorConfig sensors_[ULTRASSONIC_COUNT];
  SensorResult results_[ULTRASSONIC_COUNT];
};

#endif //
