#include "Variables.h"
#include "Pins.h"
#include <Arduino.h>

bool isRunning = false;
void (*state)(unsigned long);
void (*previousState)(unsigned long);

Servo cameraServo, steeringServo, esc;

CmdMessenger rPiCmdMessenger = CmdMessenger(Serial,',',';','/');
CmdMessenger mpuCmdMessenger = CmdMessenger(Serial2,',',';','/');

double setPointZero = 0;

double targetDirection, targetDistance = 999999;

double cameraDirection;
double cameraServoPosition;
double cameraServoKp = 1, cameraServoKi = 0, cameraServoKd = 0;

double steeringServoPosition;
double steeringServoKp = 0.5, steeringServoKi = 0, steeringServoKd = 0;

double linearSpeed, linearSpeedLock;
double speedKp = 1, speedKi = 0, speedKd = 0;

Vector2 currentVelocity;
Transform currentTransform(3, 3, 0);

LinkedList<Target> targets;
Target currentTarget;

ExecutionFlags computePid;
ExecutionFlags actuatorsWrite;