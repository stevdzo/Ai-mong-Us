#include "AgentEvade.h"
#include "AgentPursuit.h"
#include "AgentWander.h"

AgentEvade* AgentEvade::instance() {

	static AgentEvade instance;

	return &instance;
}

void AgentEvade::enter(Agent* state_machine) {

	//std::cout << "Agent " << state_machine->get_id() << " evade on " << std::endl;

	target = state_machine->steering()->get_target_agent_1();

	state_machine->steering()->obstacle_avoidance_on();
	state_machine->steering()->evade_on(target);
};

void AgentEvade::execute(Agent* state_machine) {

	if (state_machine->player_disappear()) {

		state_machine->change_state(AgentWander::instance());
	}

	if (state_machine->is_dead()) {

		state_machine->steering()->obstacle_avoidance_off();
		state_machine->change_state(AgentWander::instance());
	}
};

void AgentEvade::exit(Agent* state_machine) {

	//std::cout << "Agent " << state_machine->get_id() << " evade on " << std::endl;
	state_machine->steering()->evade_off();
};