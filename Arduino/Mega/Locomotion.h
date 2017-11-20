#ifndef LOCOMOTION_H
#define LOCOMOTION_H

#include <Servo.h>

class Locomotion
{
public:
  Locomotion(uint8_t escPin, uint8_t steeringServoPin);
  
  void move(float speed, float direction);
  void stop();
  void rotate(float direction);
  
private:
  Servo esc_;
  Servo steeringServo_;
  uint8_t currentSpeed_;
  uint8_t currentAngle_;
};

#endif //LOCOMOTION_H
