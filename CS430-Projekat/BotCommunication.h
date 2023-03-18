#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H
#pragma warning (disable:4786)

#include <set>
#include <vector>
#include "Telegram.h"

class BaseGameEntity;
class Agent;

#define dispatch BotCommunication::instance()

class BotCommunication {

private:

    std::set<Telegram> priority_queue;

    std::vector<Agent*> agents;

    void discharge(BaseGameEntity*, const Telegram&);

    BotCommunication() {}

    BotCommunication(const BotCommunication&);
    BotCommunication& operator=(const BotCommunication&);

public:

    static BotCommunication* instance();

    void dispatch_message(double, int, int, int, const int);

    void dispatch_delayed_messages();   

    void add_agent(Agent*);

    void alert_seen(Agent*);
    void alert_gone(Agent*);
};
#endif