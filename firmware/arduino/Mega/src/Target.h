#ifndef TARGET_H
#define TARGET_H


#include "Transform.h"

struct Target : Vector2
{
    bool signal;

    Target() 
    {
    }
    
    Target(float x, float y, bool signal) : Vector2(x, y)
    {
        this->signal = signal;
    }
};

#endif //TARGET_H