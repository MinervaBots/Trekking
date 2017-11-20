#include "FollowDirectionState.h"


FollowDirectionState::FollowDirectionState(Locomotion *locomotion, ControlVariables *controlVariables) :
  locomotion_(locomotion), controlVariables_(controlVariables)
{
  
}

void FollowDirectionState::onEnter()
{
}

void FollowDirectionState::onState()
{
  // PID
  float linearSpeed;
  float angularSpeed;
  locomotion_->move(linearSpeed, angularSpeed);
}

void FollowDirectionState::onExit()
{
}
