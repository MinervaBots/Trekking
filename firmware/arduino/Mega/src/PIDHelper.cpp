#include "PIDHelper.h"

PID cameraPid(&targetDirection, &cameraServoPosition, &setPointZero, cameraServoKp, cameraServoKi, cameraServoKd, P_ON_M, DIRECT);
PID steeringPid(&targetDirection, &steeringServoPosition, &setPointZero, steeringServoKp, steeringServoKi, steeringServoKd, P_ON_M, DIRECT);
PID speedPid(&targetDistance, &linearSpeed, &setPointZero, speedKp, speedKi, speedKd, P_ON_M, REVERSE);

double speedKp = 0.07, speedKi = 0, speedKd = 0.000;
double cameraServoKp = 0.5, cameraServoKi = 0, cameraServoKd = 0.5;
double steeringServoKp = 0.75, steeringServoKi = 0, steeringServoKd = 0.9;

void setupPID()
{
    cameraPid.SetOutputLimits(-1, 1);
    steeringPid.SetOutputLimits(-1, 1);
    speedPid.SetOutputLimits(0, 1);
    setAuto(ExecutionFlags::kAll);
}

void setAuto(ExecutionFlags flags)
{
    if (flags & ExecutionFlags::kSpeed)
    {
        speedPid.SetMode(AUTOMATIC);
    }
    if (flags & ExecutionFlags::kCamera)
    {
        cameraPid.SetMode(AUTOMATIC);
    }
    if (flags & ExecutionFlags::kSteering)
    {
        steeringPid.SetMode(AUTOMATIC);
    }
}

void setManual(ExecutionFlags flags)
{
    if (flags & ExecutionFlags::kSpeed)
    {
        speedPid.SetMode(MANUAL);
    }
    if (flags & ExecutionFlags::kCamera)
    {
        cameraPid.SetMode(MANUAL);
    }
    if (flags & ExecutionFlags::kSteering)
    {
        steeringPid.SetMode(MANUAL);
    }
}

void computePID()
{
  speedPid.Compute();
  cameraPid.Compute();
  steeringPid.Compute();
}