#ifndef STATE_H
#define STATE_H


class State
{
public:
	virtual void onEnter() = 0;
	virtual void onState() = 0;
	virtual void onExit() = 0;
};


#endif //STATE_H