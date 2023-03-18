#include "AgentPursuit.h"
#include "AgentEvade.h"
#include "AgentWander.h"
#include "AgentFollowPath.h"

AgentPursuit* AgentPursuit::instance() {

	static AgentPursuit instance;

	return &instance;
}

void AgentPursuit::enter(Agent* state_machine) {

	//std::cout << "Agent " << state_machine->get_id() << " pursuit on " << std::endl;
	target = state_machine->steering()->get_target_agent_1();

	state_machine->steering()->obstacle_avoidance_on();
	state_machine->steering()->pursuit_on(target);
}

void AgentPursuit::execute(Agent* state_machine) {

	if (state_machine->player_disappear()) {

		//dispatch->alert_gone(state_machine);
	}
	if (state_machine->is_dead()) {
		
		state_machine->steering()->obstacle_avoidance_off();
		state_machine->change_state(AgentWander::instance());
	}
}

void AgentPursuit::exit(Agent* state_machine) {

	//std::cout << "Agent " << state_machine->get_id() << " pursuit off " << std::endl;
	state_machine->steering()->pursuit_off();
}