#ifndef MACROS_H
#define MACROS_H


#define CM_TO_MS(cm)                        (cm * 58.8)
#define MS_TO_CM(ms)                        (ms / 58.8)
#define MODULE(x, y)                        (sqrt(pow(x, 2) + pow(y, 2)))
#define INTEGRATION_TERM(prev, curr, dT)    ((1/2) * (curr + prev) * dT)
#define DIRECTION(v1, v2)                   atan((v1.y - v2.y) / (v1.x - v2.x))
#define CORRECT_DIRECTION(dir)              atan2(sin(dir), cos(dir))


#endif //MACROS_H