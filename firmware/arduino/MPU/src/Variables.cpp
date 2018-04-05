#include "Variables.h"

Transform transform;
Vector3 acceleration, lastAcceleration;
Vector3 velocity, lastVelocity;
Vector3 angularVelocity;
Vector3 magneticField;
Quaternion quaternion; // Extern localizado em QuaternionFilters.h

unsigned long deltaTimeMicro, lastUpdate;
float deltaTimeSec;