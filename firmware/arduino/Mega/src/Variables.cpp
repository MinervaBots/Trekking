#include "Variables.h"
#include "Pins.h"
#include <Arduino.h>

bool isRunning = false;
void (*state)(unsigned long);
void (*previousState)(unsigned long);

Servo cameraServo, steeringServo, esc;

CmdMessenger rPiCmdMessenger = CmdMessenger(Serial,',',';','/');
CmdMessenger mpuCmdMessenger = CmdMessenger(Serial1,',',';','/');

double setPointZero = 0;

RunningMedian targetDirectionFiltered = RunningMedian(5);
RunningMedian targetDistanceFiltered = RunningMedian(5);
double targetDirection, targetDistance;

double cameraServoPosition;
double cameraServoKp = 1, cameraServoKi = 0, cameraServoKd = 2;

double steeringServoPosition;
double steeringServoKp = 1, steeringServoKi = 0, steeringServoKd = 2;

double linearSpeed, linearSpeedLock;
double speedKp = 2, speedKi = 0.4, speedKd = 1;

Vector2 currentVelocity;
Transform currentTransform(3, 3, 0);

LinkedList<Target> targets;
Target currentTarget;