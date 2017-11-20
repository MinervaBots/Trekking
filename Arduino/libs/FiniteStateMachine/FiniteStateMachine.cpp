#include "FiniteStateMachine.h"
#include "Arduino.h"

FiniteStateMachine::FiniteStateMachine() :
	initialized_(false),
	currentState_(nullptr),
	transitions_(LinkedList<Transition>()),
	timedTransitions_(LinkedList<TimedTransition>())
{

}


FiniteStateMachine::~FiniteStateMachine()
{
	///for (int i = 0; i < transitions_.size(); ++i)
	//{
	//	delete transitions_.pop();
	//}
	//for (int i = 0; i < timedTransitions_.size(); ++i)
	//{
	//	delete timedTransitions_.pop();
	//}
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
	auto transition = createTransition(eventCode, fromState, toState, onTransition);
	transitions_.add(transition);
}

void FiniteStateMachine::addTimedTransition(unsigned long interval, State* fromState, State* toState, void (*onTransition)())
{
	if (fromState == nullptr || toState == nullptr)
	{
		return;
	}

	auto transition = createTransition(0, fromState, toState, onTransition);
	auto timedTransition = createTimedTransition(transition, interval);


	timedTransitions_.add(timedTransition);
}

void FiniteStateMachine::triggerEvent(int eventCode)
{
	if (initialized_)
	{
		for (int i = 0; i < transitions_.size(); i++)
		{
			Transition transition = transitions_.get(i);
			if (transition.fromState  == currentState_ && transition.eventCode == eventCode)
			{
				makeTransition(&transition);
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
		currentState_->onEnter();
	}

	currentState_->onState();
	checkTimedTransitions();
}

void FiniteStateMachine::checkTimedTransitions()
{
	for (int i = 0; i < timedTransitions_.size(); i++)
	{
		TimedTransition timedTransition = timedTransitions_.get(i);
		if (timedTransition.transition.fromState == currentState_)
		{
			if (timedTransition.start == 0)
			{
				timedTransition.start = millis();
			}
			else
			{
				unsigned long now = millis();
				if (now - timedTransition.start >= timedTransition.interval)
				{
					makeTransition(&(timedTransition.transition));
					timedTransition.start = 0;
				}
			}
		}
	}
}

void FiniteStateMachine::makeTransition(Transition* transition)
{
	transition->fromState->onExit();

	if (transition->onTransition != nullptr)
		transition->onTransition();

	transition->toState->onEnter();

	currentState_ = transition->toState;

	unsigned long now = millis();
	for (int i = 0; i < timedTransitions_.size(); i++)
	{
		TimedTransition timedTransition = timedTransitions_.get(i);
		if (timedTransition.transition.fromState == currentState_)
			timedTransition.start = now;
	}
}

FiniteStateMachine::Transition FiniteStateMachine::createTransition(int eventCode, State* fromState, State* toState, void (*onTransition)())
{
	auto transition = Transition();
	transition.eventCode = eventCode;
	transition.fromState = fromState;
	transition.toState = toState;
	transition.onTransition = onTransition;
	return transition;
}

FiniteStateMachine::TimedTransition FiniteStateMachine::createTimedTransition(Transition transition, unsigned long interval)
{
	auto timedTransition = TimedTransition();
	timedTransition.transition = transition;
	timedTransition.interval = interval;
	return timedTransition;
}