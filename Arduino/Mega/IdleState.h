#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include "State.h"
#include "Locomotion.h"

class IdleState : public State
{
public:
  IdleState(Locomotion *locomotion);
  
  void onEnter() override;
  void onState() override;
  void onExit() override;
  
private:
  Locomotion *locomotion_;
};

#endif //IDLE_STATE_H
