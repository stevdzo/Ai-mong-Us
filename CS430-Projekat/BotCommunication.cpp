#include "BotCommunication.h"
#include "BaseGameEntity.h"
#include "Agent.h"
#include "EntityManager.h"
#include "AgentPursuit.h"
#include "AgentEvade.h"
#include "AgentFollowPath.h"
#include "AgentWander.h"

BotCommunication* BotCommunication::instance() {

    static BotCommunication instance;

    return &instance;
}

void BotCommunication::discharge(BaseGameEntity* pReceiver,
    const Telegram& telegram)
{
    /*if (!pReceiver->HandleMessage(telegram)) {

        cout << "\nMessage not handled";
    }*/
}

void BotCommunication::dispatch_message(double  delay,
    int    sender,
    int    receiver,
    int    msg,
    const int ExtraInfo)
{

    BaseGameEntity* pSender = entity_manager->get_entity_from_id(sender);
    BaseGameEntity* pReceiver = entity_manager->get_entity_from_id(receiver);

    if (pReceiver == NULL) {
        std::cout << "\nWarning! No Receiver with ID of " << receiver << " found";

        return;
    }

    Telegram telegram(sender, receiver, msg);

    std::cout << "\nInstant telegram dispatched at time: " << 0
        << " by " << pSender->get_id() << " for " << pReceiver->get_id()
        << ". Msg is "; //<< msg_to_str(msg);

       discharge(pReceiver, telegram);
}

void BotCommunication::add_agent(Agent* a) {

    agents.push_back(a);
}

void BotCommunication::alert_seen(Agent* seeker) {

    for (auto& i : agents) {
         
        if (i->type != bot_type::evader)      
        i->change_state(AgentPursuit::instance());

        if (i == seeker) {

            std::cout << i->get_id() << " : Found him." << std::endl;
        }
        else {
                    
            std::cout << i->get_id() << " : Ok." << std::endl;
        }
    }
}

void BotCommunication::alert_gone(Agent* seeker) {

    for (auto& i : agents) {
       
        //if (i->type == bot_type::path_follower) i->change_state(AgentFollowPath::instance());

        if (i == seeker) {

            std::cout << i->get_id() << " : He's gone." << std::endl;
            i->change_state(AgentFollowPath::instance());
        }
        else {
          
            std::cout << i->get_id() << " : Ok." << std::endl;
        }
    }
}