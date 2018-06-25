#ifndef VARIABLES_H
#define VARIABLES_H

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

extern double cameraDirection;
extern double cameraServoPosition;

extern double steeringServoPosition;

extern double linearSpeed, linearSpeedLock;

extern Vector2 currentVelocity;
extern Transform currentTransform;
extern LinkedList<Target> targets;
extern Target currentTarget;

extern SonicArray sonicArray;

enum ExecutionFlags
{
    kNone = 0,
    kSpeed = 1,
    kCamera = 2,
    kSteering = 4,
    kAll = 7
};

extern ExecutionFlags actuatorsWrite;

#endif //VARIABLES_H
