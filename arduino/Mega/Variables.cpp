#include "Variables.h"

bool isRunning = false;
void (*state)(unsigned long);

double setPointZero = 0;

RunningMedian targetDirectionFiltered = RunningMedian(5);
RunningMedian targetDistanceFiltered = RunningMedian(5);
double targetDirection, targetDistance;

double cameraServoPosition;
double cameraServoKp = 1, cameraServoKi = 0, cameraServoKd = 2;

double steeringServoPosition;
double steeringServoKp = 1, steeringServoKi = 0, steeringServoKd = 2;

double linearSpeed;
double speedKp = 2, speedKi = 0.4, speedKd = 1;

Transform currentTransform(3, 3, 0);

LinkedList<Vector2> targets;
Vector2* currentTarget;
