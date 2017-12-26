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

float UltrassonicArray::evaluate()
{
  float result = 0;
  if(evaluateSensor(rightSensor_) < ULTRASSONIC_MAX_DIST)
  {
    result = rightSensor_.weight;
  }
  else if(evaluateSensor(leftSensor_) < ULTRASSONIC_MAX_DIST)
  {
    result = leftSensor_.weight;
  }

  if(evaluateSensor(forwardSensor_) < ULTRASSONIC_MAX_DIST)
  {
    result += forwardSensor_.weight;
  }
  if(evaluateSensor(forwardRightSensor_) < ULTRASSONIC_MAX_DIST)
  {
    result += forwardRightSensor_.weight;
  }
  if(evaluateSensor(forwardLeftSensor_) < ULTRASSONIC_MAX_DIST)
  {
    result += forwardRightSensor_.weight;
  }
  return result;
}

