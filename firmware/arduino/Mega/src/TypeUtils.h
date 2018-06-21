#ifndef TYPE_UTILS_H
#define TYPE_UTILS_H

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/*
float lerp(float a, float b, float t)
{
    return (1-t)*a + t*b;
}
*/
#endif
