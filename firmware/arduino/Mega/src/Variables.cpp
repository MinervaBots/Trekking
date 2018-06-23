#include "Variables.h"
#include "Pins.h"
#include <Arduino.h>

bool isRunning = false;
void (*state)(unsigned long);
void (*previousState)(unsigned long);

Servo cameraServo, steeringServo, esc;

CmdMessenger rPiCmdMessenger = CmdMessenger(Serial,',',';','/');
//CmdMessenger mpuCmdMessenger = CmdMessenger(Serial2,',',';','/');

double setPointZero = 0;

double targetDirection, targetDistance = 0;

double cameraDirection;
double cameraServoPosition;
double cameraServoKp = 0.5, cameraServoKi = 0, cameraServoKd = 0.5;

double steeringServoPosition;
double steeringServoKp = 0.75, steeringServoKi = 0, steeringServoKd = 0.9;

double linearSpeed = 0, linearSpeedLock;
double speedKp = 0.07, speedKi = 0, speedKd = 0.000;

Vector2 currentVelocity;
Transform currentTransform(3, 3, 0);

LinkedList<Target> targets;
Target currentTarget;

ExecutionFlags computePid;
ExecutionFlags actuatorsWrite;