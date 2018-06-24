#ifndef PID_HELPER_H
#define PID_HELPER_H

#include <PID_v1.h>
#include "Variables.h"

extern PID cameraPid;
extern PID steeringPid;
extern PID speedPid;


extern double speedKp, speedKi, speedKd;
extern double cameraServoKp, cameraServoKi, cameraServoKd;
extern double steeringServoKp, steeringServoKi, steeringServoKd;


void setupPID();
void setAuto(ExecutionFlags flags = ExecutionFlags::kAll);
void setManual(ExecutionFlags flags = ExecutionFlags::kAll);
void computePID();

#endif