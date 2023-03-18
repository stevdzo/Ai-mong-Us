#ifndef STATE_H
#define STATE_H

#include "Agent.h"
#include "SteeringBehaviors.h"
#include "BotCommunication.h"

class State {

public:

	State() {}

	virtual void enter(Agent*) {}
	virtual void execute(Agent*) { /*printf("State Execute\n");*/ }
	virtual void exit(Agent*) {}

	//virtual bool message(Agent*) = 0;
};
#endif