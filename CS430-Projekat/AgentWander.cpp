#include "AgentWander.h"
#include "AgentPursuit.h"
#include "AgentEvade.h"

AgentWander* AgentWander::instance() {

	static AgentWander instance;

	return &instance;
}

void AgentWander::enter(Agent* state_machine) {

	//std::cout << "Agent " << state_machine->get_id() << " wander on " << std::endl;
	state_machine->steering()->wander_on();
};

void AgentWander::execute(Agent* state_machine) {

	if (state_machine->player_spotted() && !state_machine->is_dead()) {
	
		dispatch->alert_seen(state_machine);

		/*if (state_machine->type != bot_type::evader) state_machine->change_state(AgentPursuit::instance());
		else state_machine->change_state(AgentEvade::instance());*/
	}
};

void AgentWander::exit(Agent* state_machine) {

	//std::cout << "Agent " << state_machine->get_id() << " wander off " << std::endl;
	state_machine->steering()->wander_off();
};