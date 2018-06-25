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

double targetDirection, targetDistance = 0;

double cameraDirection;
double cameraServoPosition;

double steeringServoPosition;

double linearSpeed = 0, linearSpeedLock;

Vector2 currentVelocity;
Transform currentTransform(3, 3, 0);

LinkedList<Target> targets;
Target currentTarget;

ExecutionFlags actuatorsWrite;