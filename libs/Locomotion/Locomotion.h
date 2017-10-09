#ifndef LOCOMOTION_H
#define LOCOMOTION_H

#include <Servo.h>

class Locomotion
{
public:
  Locomotion(uint8_t motorPin, uint8_t servoPin);
  
  void move(float speed, float direction);
  void stop();
  void rotate(float direction);
  
private:
  uint8_t motorPin_;
  Servo servo_;
  uint8_t currentAngle_;
};

#endif //LOCOMOTION_H
