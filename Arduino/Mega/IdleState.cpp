#include "IdleState.h"

IdleState::IdleState(Locomotion *locomotion) : locomotion_(locomotion)
{
  
}

void IdleState::onEnter()
{
  locomotion_->stop();
}

void IdleState::onState()
{
}

void IdleState::onExit()
{
}
