#ifndef TRANSFORM_H
#define TRANSFORM_H

struct Transform
{
  float x, y;
  float heading;
  
  Transform()
  {
    x = 0;
    y = 0;
    heading = 0;
  }
  
  Transform(float x, float y, float heading)
  {
    this->x = x;
    this->y = y;
    this->heading = heading;
  }
};

#endif //TRANSFORM_H
