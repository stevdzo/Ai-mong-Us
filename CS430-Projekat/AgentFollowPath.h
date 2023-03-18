#ifndef AGENTFOLLOWPATH_H
#define AGENTFOLLOWPATH_H

#include "State.h"

class AgentFollowPath : public State {

private:

    AgentFollowPath() {};
    Agent* target;

public:

    static AgentFollowPath* instance();

    void execute(Agent*) override;

    void enter(Agent*) override;

    void exit(Agent*) override;
};
#endif