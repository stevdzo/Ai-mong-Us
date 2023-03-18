#include "AgentFollowPath.h"
#include "AgentWander.h"
#include "AgentPursuit.h"

AgentFollowPath* AgentFollowPath::instance() {

	static AgentFollowPath instance;

	return &instance;
}

void AgentFollowPath::enter(Agent* state_machine) {

	//std::cout << "Agent " << state_machine->get_id() << " follow path on " << std::endl;
	state_machine->steering()->obstacle_avoidance_on();
	state_machine->steering()->follow_path_on();
}

void AgentFollowPath::execute(Agent* state_machine) {

	if (state_machine->player_spotted()) {

		dispatch->alert_seen(state_machine);
		//state_machine->change_state(AgentPursuit::instance());
	} 

	if (state_machine->is_dead()) {

		state_machine->change_state(AgentWander::instance());
		state_machine->steering()->obstacle_avoidance_off() ;
	}
}

void AgentFollowPath::exit(Agent* state_machine) {

	//std::cout << "Agent " << state_machine->get_id() << " follow path off " << std::endl;
	state_machine->steering()->follow_path_off();
}