#include "UltrassonicArray.h"
#include "Macros.h"

UltrassonicArray::UltrassonicArray()
{

}

void UltrassonicArray::setForward(unsigned char trigger, unsigned char echo, float weight)
{
  forwardSensor_ = SensorConfig(trigger, echo, weight);
}

void UltrassonicArray::setForwardLeft(unsigned char trigger, unsigned char echo, float weight)
{
  forwardLeftSensor_ = SensorConfig(trigger, echo, weight);
}

void UltrassonicArray::setForwardRight(unsigned char trigger, unsigned char echo, float weight)
{
  forwardRightSensor_ = SensorConfig(trigger, echo, weight);
}

void UltrassonicArray::setLeft(unsigned char trigger, unsigned char echo, float weight)
{
  leftSensor_ = SensorConfig(trigger, echo, weight);
}

void UltrassonicArray::setRight(unsigned char trigger, unsigned char echo, float weight)
{
  rightSensor_ = SensorConfig(trigger, echo, weight);
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

