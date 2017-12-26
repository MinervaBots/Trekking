#ifndef ULTRASSONIC_ARRAY_H
#define ULTRASSONIC_ARRAY_H

#include <Arduino.h>
#include "Constants.h"

class UltrassonicArray
{
public:
  UltrassonicArray();
  void setForward(unsigned char trigger, unsigned char echo, float weight);
  void setForwardLeft(unsigned char trigger, unsigned char echo, float weight);
  void setForwardRight(unsigned char trigger, unsigned char echo, float weight);
  void setLeft(unsigned char trigger, unsigned char echo, float weight);
  void setRight(unsigned char trigger, unsigned char echo, float weight);
  
  float evaluate();

private:
  struct SensorConfig
  {
    unsigned char trigger, echo;
    float weight;
    
    SensorConfig(unsigned char trigger, unsigned char echo, float weight)
    {
      this->trigger = trigger;
      this->echo = echo;
      this->weight = weight;
  
      pinMode(trigger, OUTPUT);
      pinMode(echo, INPUT);
    }
    
    SensorConfig()
    {
      
    }
  };

  unsigned int sensorCount_;
  SensorConfig forwardSensor_, forwardLeftSensor_, forwardRightSensor_, rightSensor_, leftSensor_;

  static float evaluateSensor(SensorConfig sensor);
};

#endif //
