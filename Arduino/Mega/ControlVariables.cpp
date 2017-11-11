#include "ControlVariables.h"

ControlVariables::ControlVariables()
{
  direction_ = 0.0;
  maxVelocity_ = 1.0;
}

ControlVariables::~ControlVariables()
{

}

float ControlVariables::getDirection()
{
  return direction_;
}

void ControlVariables::setDirection(float value)
{
  direction_ = value;
}

float ControlVariables::getMaxVelocity()
{
  return maxVelocity_;
}

void ControlVariables::setMaxVelocity(float value)
{
  maxVelocity_ = value;
}
