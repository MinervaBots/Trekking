#include <Arduino.h>

#include "Locomotion.h"


Locomotion::Locomotion(uint8_t motorPin, uint8_t servoPin) :
  motorPin_(motorPin)
{
  servo_.attach(servoPin);
}

void Locomotion::move(float speed, float direction)
{
  speed = constrain(speed, -1, 1);
  direction = constrain(direction, -1, 1);

  int pwm = map(speed, -1, 1, 0, 255);
  analogWrite(motorPin_, pwm);

  rotate(direction);
}

void Locomotion::stop()
{
  move(0, currentAngle_);
}

void Locomotion::rotate(float direction)
{
  currentAngle_ = map(direction, -1, -1, 0, 180);
  servo_.write(currentAngle_);
}

