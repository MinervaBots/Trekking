#include "UltrassonicArray.h"
#include "Macros.h"

UltrassonicArray::UltrassonicArray()
{

}

void UltrassonicArray::setForward(unsigned char triggerPin, unsigned char echo, float weight)
{
  forwardSensor_ = SensorConfig(triggerPin, echo, weight);
}

void UltrassonicArray::setForwardLeft(unsigned char triggerPin, unsigned char echo, float weight)
{
  forwardLeftSensor_ = SensorConfig(triggerPin, echo, weight);
}

void UltrassonicArray::setForwardRight(unsigned char triggerPin, unsigned char echo, float weight)
{
  forwardRightSensor_ = SensorConfig(triggerPin, echo, weight);
}

void UltrassonicArray::setLeft(unsigned char triggerPin, unsigned char echo, float weight)
{
  leftSensor_ = SensorConfig(triggerPin, echo, weight);
}

void UltrassonicArray::setRight(unsigned char triggerPin, unsigned char echo, float weight)
{
  rightSensor_ = SensorConfig(triggerPin, echo, weight);
}

bool UltrassonicArray::evaluate(float *value)
{
  *value = 0;
  bool result = false;
 
  if(evaluateSensor(rightSensor_) < ULTRASSONIC_MAX_DIST)
  {
    *value = rightSensor_.weight;
    result = true;
  }
  else if(evaluateSensor(leftSensor_) < ULTRASSONIC_MAX_DIST)
  {
    *value = leftSensor_.weight;
    result = true;
  }

  if(evaluateSensor(forwardSensor_) < ULTRASSONIC_MAX_DIST)
  {
    *value += forwardSensor_.weight;
    result = true;
  }
  if(evaluateSensor(forwardRightSensor_) < ULTRASSONIC_MAX_DIST)
  {
    *value += forwardRightSensor_.weight;
    result = true;
  }
  if(evaluateSensor(forwardLeftSensor_) < ULTRASSONIC_MAX_DIST)
  {
    *value += forwardRightSensor_.weight;
    result = true;
  }
  
  return result;
}

float UltrassonicArray::evaluateSensor(SensorConfig sensor)
{
  sensor.trigger();
  int duration = pulseIn(sensor.echoPin, HIGH);
  if (duration == 0)
  {
	  return ULTRASSONIC_MAX_DIST;
  }
  return duration / 29.4 / 2;
}