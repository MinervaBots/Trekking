#ifndef VARIABLES_H
#define VARIABLES_H

#include <RunningMedian.h>
#include <LinkedList.h>

#include "Transform.h"
#include "UltrassonicArray.h"

extern bool isRunning;
extern void (*state)(unsigned long);

extern double setPointZero;

extern double targetDirection, targetDistance;
extern RunningMedian targetDirectionFiltered, targetDistanceFiltered;   

extern double cameraServoPosition;
extern double cameraServoKp, cameraServoKi, cameraServoKd;

extern double steeringServoPosition;
extern double steeringServoKp, steeringServoKi, steeringServoKd;

extern double linearSpeed;
extern double speedKp, speedKi, speedKd;

extern Transform currentTransform;
extern LinkedList<Vector2> targets;
extern Vector2* currentTarget;

extern UltrassonicArray ultrassonicArray;

#endif //VARIABLES_H
