#ifndef VARIABLES_H
#define VARIABLES_H

#include <RunningMedian.h>
#include <LinkedList.h>
#include <CmdMessenger.h>
#include <Servo.h>

#include "Transform.h"
#include "SonicArray.h"
#include "Target.h"

extern bool isRunning;
extern void (*state)(unsigned long);
extern void (*previousState)(unsigned long);

extern Servo cameraServo, steeringServo, esc;
extern CmdMessenger rPiCmdMessenger, mpuCmdMessenger;

extern double setPointZero;

extern double targetDirection, targetDistance;
extern RunningMedian targetDirectionFiltered, targetDistanceFiltered;   

extern double cameraServoPosition;
extern double cameraServoKp, cameraServoKi, cameraServoKd;

extern double steeringServoPosition;
extern double steeringServoKp, steeringServoKi, steeringServoKd;

extern double linearSpeed, linearSpeedLock;
extern double speedKp, speedKi, speedKd;

extern Vector2 currentVelocity;
extern Transform currentTransform;
extern LinkedList<Target> targets;
extern Target currentTarget;

extern SonicArray sonicArray;

#endif //VARIABLES_H
