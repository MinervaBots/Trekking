#ifndef FOLLOW_DIRECTION_STATE_H
#define FOLLOW_DIRECTION_STATE_H

#include "State.h"
#include "Locomotion.h"
#include "ControlVariables.h"

class FollowDirectionState : public State
{
public:
  FollowDirectionState(Locomotion *locomotion, ControlVariables *controlVariables);
  void onEnter() override;
  void onState() override;
  void onExit() override;
  
private:
  Locomotion *locomotion_;
  ControlVariables *controlVariables_;
};

#endif //FOLLOW_DIRECTION_STATE_H
