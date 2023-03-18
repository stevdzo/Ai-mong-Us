#ifndef AGENTEVADE_H
#define AGENTEVADE_H

#include "State.h"

class AgentEvade : public State {

private:

    AgentEvade() {};
    Agent* target;

public:

    static AgentEvade* instance();

    void execute(Agent*) override;

    void enter(Agent*) override;

    void exit(Agent*) override;

    //bool message(Agent*) override;
};
#endif