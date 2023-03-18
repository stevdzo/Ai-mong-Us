#ifndef AGENTPURSUIT_H
#define AGENTPURSUIT_H

#include "State.h"

class AgentPursuit : public State {

private:

    Agent* target;
    AgentPursuit() {}

public:

    static AgentPursuit* instance();

    void enter(Agent*) override;

    void exit(Agent*) override;

    void execute(Agent*) override;
};
#endif