#ifndef WANDERSTATE_H
#define WANDERSTATE_H

#include "State.h"

class AgentWander : public State {

private:

    AgentWander() {}

public:

    static AgentWander* instance();

    void execute(Agent*) override;

    void enter(Agent*) override;

    void exit(Agent*) override;

};
#endif