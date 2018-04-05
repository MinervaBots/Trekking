#ifndef VARIABLES_H
#define VARIABLES_H

#include "Vector.h"
#include "Transform.h"
#include <QuaternionFilters.h>

extern Transform transform;
extern Vector3 acceleration, lastAcceleration;
extern Vector3 velocity, lastVelocity;
extern Vector3 angularVelocity;
extern Vector3 magneticField;

extern unsigned long deltaTimeMicro, lastUpdate;
extern float deltaTimeSec;




#endif //VARIABLES_H
