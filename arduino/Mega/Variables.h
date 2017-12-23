#ifndef VARIABLES_H
#define VARIABLES_H

#include "Transform.h"

extern bool isRunning;

extern double setPointZero;

extern double targetDirection, targetDistance;

extern double cameraServoPosition;
extern double cameraServoKp, cameraServoKi, cameraServoKd;

extern double steeringServoPosition;
extern double steeringServoKp, steeringServoKi, steeringServoKd;

extern double linearSpeed;
extern double speedKp, speedKi, speedKd;

extern Transform currentTransform;

#endif //VARIABLES_H
