#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include "State.h"
#include "LinkedList.h"

class FiniteStateMachine
{
public:
	FiniteStateMachine();
	~FiniteStateMachine();

	void addTransition(int eventCode, State* fromState, State* toState, void (*onTransition)() = nullptr);
	void addTimedTransition(unsigned long interval, State* fromState, State* toState, void (*onTransition)() = nullptr);
	void triggerEvent(int eventCode);
	void runMachine();

private:
	struct Transition
	{
		int eventCode;
		State* fromState;
		State* toState;
    	void (*onTransition)();

    	Transition(int eventCode, State* fromState, State* toState, void (*onTransition)()) : eventCode(eventCode), fromState(fromState), toState(toState), onTransition(onTransition)
    	{}
	};

	struct TimedTransition
	{
		Transition transition;
		unsigned long start;
		unsigned long interval;

    	TimedTransition(Transition transition, unsigned long interval) : transition(transition), start(start), interval(interval)
    	{}
	};


	bool initialized_;
	State* currentState_;
	LinkedList<Transition*> transitions_;
	LinkedList<TimedTransition*> timedTransitions_;


	void checkTimedTransitions();
	void makeTransition(Transition* transition);
};


#endif //FINITE_STATE_MACHINE_H