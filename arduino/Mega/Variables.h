#ifndef VARIABLES_H
#define VARIABLES_H

#include "Transform.h"

extern bool isRunning;

extern float setPointZero;

extern float targetDirection, targetDistance;

extern float cameraServoPosition;
extern float cameraServoKp, cameraServoKi, cameraServoKd;

extern float steeringServoPosition;
extern float steeringServoKp, steeringServoKi, steeringServoKd;

extern float linearSpeed;
extern float speedKp, speedKi, speedKd;

extern Transform currentTransform;

#endif //VARIABLES_H
