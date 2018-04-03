#ifndef _QUATERNIONFILTERS_H_
#define _QUATERNIONFILTERS_H_

#include <Arduino.h>

struct Quaternion
{
    float W;
    float X;
    float Y;
    float Z;

    void toEulerAngles(float *yaw, float *pitch, float *roll)
    {
        // yaw (z-axis rotation)
        float siny = +2.0 * (W * Z + X * Y);
        float cosy = +1.0 - 2.0 * (Y * Y + Z * Z);
        *yaw = (float)atan2(siny, cosy);

        // pitch (y-axis rotation)
        float sinp = +2.0 * (W * Y - Z * X);
        if (fabs(sinp) >= 1)
            *pitch = (float)copysign(M_PI / 2, sinp); // use 90 degrees if out of range
        else
            *pitch = (float)asin(sinp);

        // roll (x-axis rotation)
        float sinr = +2.0 * (W * X + Y * Z);
        float cosr = +1.0 - 2.0 * (X * X + Y * Y);
        *roll = (float)atan2(sinr, cosr);
    }
};

void MadgwickQuaternionUpdate(float ax, float ay, float az,
                              float gx, float gy, float gz,
                              float mx, float my, float mz,
                              float deltaTimeSec);
void MahonyQuaternionUpdate(float ax, float ay, float az,
                            float gx, float gy, float gz,
                            float mx, float my, float mz,
                            float deltaTimeSec);

extern Quaternion quaternion;

#endif // _QUATERNIONFILTERS_H_
