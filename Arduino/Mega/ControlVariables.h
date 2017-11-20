#ifndef CONTROL_VARIABLES_H
#define CONTROL_VARIABLES_H


struct TargetPosition
{
  int x, y, w, h;
};

class ControlVariables
{
public:
  ControlVariables();
  ~ControlVariables();
  
  float getDirection();
  void setDirection(float value);

  float getMaxVelocity();
  void setMaxVelocity(float value);
  
private:
  float direction_;
  float maxVelocity_;
};



#endif

