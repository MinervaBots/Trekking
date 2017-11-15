#include "FiniteStateMachine.h"
#include "Arduino.h"

FiniteStateMachine::FiniteStateMachine() :
	initialized_(false),
	currentState_(nullptr),
	transitions_(LinkedList<Transition*>()),
	timedTransitions_(LinkedList<TimedTransition*>())
{

}


FiniteStateMachine::~FiniteStateMachine()
{
	for (int i = 0; i < transitions_.size(); ++i)
	{
		delete transitions_.pop();
	}
	for (int i = 0; i < timedTransitions_.size(); ++i)
	{
		delete timedTransitions_.pop();
	}
}

void FiniteStateMachine::addTransition(int eventCode, State* fromState, State* toState, void (*onTransition)())
{
	if (fromState == nullptr || toState == nullptr)
	{
		return;
	}
	if (currentState_ == nullptr)
	{
		currentState_ = fromState;
	}
	transitions_.add(Transition(eventCode, fromState, toState, onTransition));
}

void FiniteStateMachine::addTimedTransition(unsigned long interval, State* fromState, State* toState, void (*onTransition)())
{
	if (fromState == nullptr || toState == nullptr)
	{
		return;
	}

	Transition transition(eventCode, fromState, toState, onTransition);
	TimedTransition timedTransition(transition, interval);


	timedTransitions_.add(timedTransition);
}

void FiniteStateMachine::triggerEvent(int eventCode)
{
	if (initialized_)
	{
		for (int i = 0; i < transitions_.size(); i++)
		{
			State state = transitions_.get(i);
			if (state.fromState  == currentState_ && state.eventCode == eventCode)
			{
				makeTransition(&state);
				return;
			}
		}
	}
}

void FiniteStateMachine::runMachine()
{
	if (!initialized_)
	{
		initialized_ = true;
		if (currentState_->onEnter != nullptr)
			currentState_->onEnter();
	}

	if (currentState_->onState != nullptr)
		currentState_->onState();

	checkTimedTransitions();
}

void FiniteStateMachine::checkTimedTransitions()
{
	for (int i = 0; i < timedTransitions_.size(); i++)
	{
		TimedTransition* timedTransition = &timedTransitions_.get(i);
		if (timedTransition->transition.fromState == currentState_)
		{
			if (timedTransition->start == 0)
			{
				timedTransition->start = millis();
			}
			else
			{
				unsigned long now = millis();
				if (now - timedTransition->start >= timedTransition->interval)
				{
					makeTransition(&(timedTransition->transition));
					timedTransition->start = 0;
				}
			}
		}
	}
}

void FiniteStateMachine::makeTransition(Transition* transition)
{
	if (transition->fromState->onExit() != nullptr)
		transition->fromState->onExit();

	if (transition->onTransition != nullptr)
		transition->onTransition();

	if (transition->toState->onEnter != nullptr)
		transition->toState->onEnter();

	currentState_ = transition->toState;

	unsigned long now = millis();
	for (int i = 0; i < timedTransitions_.size(); i++)
	{
		TimedTransition* timedTransition = &timedTransitions_.get(i);
		if (timedTransition->transition.fromState == currentState_)
			timedTransition->start = now;
	}
}