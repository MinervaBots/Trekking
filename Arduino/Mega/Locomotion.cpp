#include <Arduino.h>

#include "Locomotion.h"


Locomotion::Locomotion(uint8_t escPin, uint8_t steeringServoPin)
{
  esc_.attach(escPin);
  steeringServo_.attach(steeringServoPin);
}

void Locomotion::move(float speed, float direction)
{
  speed = constrain(speed, -1, 1);

  currentSpeed_ = map(direction, -1, -1, 0, 180);
  esc_.write(currentSpeed_);
  
  direction = constrain(direction, -1, 1);
  rotate(direction);
}

void Locomotion::stop()
{
  move(0, currentAngle_);
}

void Locomotion::rotate(float direction)
{
  currentAngle_ = map(direction, -1, -1, 0, 180);
  steeringServo_.write(currentAngle_);
}
