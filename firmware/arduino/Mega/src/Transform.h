#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Macros.h"

struct Vector2
{
  float x, y;

  Vector2()
  {
    this->x = 0;
    this->y = 0;
  }
  
  Vector2(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  float distance(float x, float y)
  {
    return MODULE(this->x - x, this->y - y);
  }

  float magnitude()
  {
    return MODULE(this->x, this->y);
  }
};

struct Transform
{
  Vector2 position;
  float heading;
  
  Transform()
  {
    position = Vector2();
    heading = 0;
  }
  
  Transform(float x, float y, float heading)
  {
    position = Vector2(x, y);
    this->heading = heading;
  }
};

#endif //TRANSFORM_H
