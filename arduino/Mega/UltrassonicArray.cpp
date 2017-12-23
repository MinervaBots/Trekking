#include "UltrassonicArray.h"
#include "Macros.h"

UltrassonicArray::UltrassonicArray() : sensorCount_(0)
{
  
}

bool UltrassonicArray::add(float direction, unsigned char trigger, unsigned char echo)
{
  if(sensorCount_ == ULTRASSONIC_COUNT)
  {
    return false;
  }
  sensors_[sensorCount_] = SensorConfig(trigger, echo, direction);
  sensorCount_++;
  return true;
}

SensorResult UltrassonicArray::evaluate(int sensorId)
{
  //assert(sensorId < sensorCount_);
  
  SensorResult result;
  
  if(sensorId >= sensorCount_)
  {
    result.distance = 160000;
    result.direction = 160000;
  }
  else
  {
    auto sensor = sensors_[sensorId];

    // Na versão de 2018.1 isso será substituido por um
    // sistema usando interrupções.
    //
    // Isso vai retirar todo esse delay entre o trigger
    // e receber o echo.
    result.direction = sensor.direction;
    digitalWrite(sensor.trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(sensor.trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(sensor.trigger, LOW);
    
    long duration = pulseIn(sensor.echo, HIGH, ULTRASSONIC_TIMEOUT);
    if (duration == 0)
    {
      result.distance = 160000;
    }
    else
    {
      result.distance = MS_TO_CM(duration);
    }
  }
  return result;
}

