#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <Arduino.h>

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
    return sqrt(pow(this->x - x, 2) + pow(this->y - y, 2));
  }

  float magnitude()
  {
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
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
