#ifndef ULTRASSONIC_ARRAY_H
#define ULTRASSONIC_ARRAY_H

#include <Arduino.h>
#include "Constants.h"

class UltrassonicArray
{
public:
  UltrassonicArray();
  void setForward(unsigned char triggerPin, unsigned char echoPin, float weight);
  void setForwardLeft(unsigned char triggerPin, unsigned char echoPin, float weight);
  void setForwardRight(unsigned char triggerPin, unsigned char echoPin, float weight);
  void setLeft(unsigned char triggerPin, unsigned char echoPin, float weight);
  void setRight(unsigned char triggerPin, unsigned char echoPin, float weight);
  
  bool evaluate(float *value);

private:
  struct SensorConfig
  {
    unsigned char triggerPin, echoPin;
    float weight;
    
    SensorConfig(unsigned char triggerPin, unsigned char echoPin, float weight)
    {
      this->triggerPin = triggerPin;
      this->echoPin = echoPin;
      this->weight = weight;
  
      pinMode(triggerPin, OUTPUT);
      pinMode(echoPin, INPUT);
    }
    
    SensorConfig()
    {
      
    }

    void trigger()
    {
      digitalWrite(triggerPin, LOW);
      delayMicroseconds(2);
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);
    }
  };

  unsigned int sensorCount_;
  SensorConfig forwardSensor_, forwardLeftSensor_, forwardRightSensor_, rightSensor_, leftSensor_;

  static float evaluateSensor(SensorConfig sensor);
};

#endif //
