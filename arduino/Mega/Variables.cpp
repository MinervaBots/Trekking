#include "Variables.h"

bool isRunning = false;

float setPointZero = 0;

float targetDirection, targetDistance;

float cameraServoPosition;
float cameraServoKp = 1, cameraServoKi = 0, cameraServoKd = 2;

float steeringServoPosition;
float steeringServoKp = 1, steeringServoKi = 0, steeringServoKd = 2;

float linearSpeed;
float speedKp = 2, speedKi = 0.4, speedKd = 1;

Transform currentTransform(3, 3, 0);
