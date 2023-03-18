#include "Input.h"
#include "GameWorld.h"
#include "SteeringBehaviors.h"

Input::Input() {}

Input::Input(Agent* _agent, Graph* _graph) : agent(_agent), graph(_graph) {}

Input::Input(Agent* _agent) : agent(_agent) {}

Input::~Input() {}

void Input::mouse(int button, int state, int x, int y) {

	if (button == GLUT_RIGHT_BUTTON) {

		if (state == GLUT_DOWN) {

			if (agent->is_player()) { 

				if (agent->find_path(x, y)) agent->steering()->follow_path_on();
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON) {
	
		if (state == GLUT_DOWN) {
		
			if (agent->is_player()) {

				agent->player_attack(x, y);
				agent->hat_click(x, y);
			}
		}
	}
}

void Input::keyboard_press(unsigned char key, int x, int y) {

	agent->set_cannot_walk(false);
	agent->steering()->follow_path_off();
	agent->get_pin()->is_visible(false);
	key_down[key] = true;
}

void Input::keyboard_up(unsigned char key, int x, int y) {

	key_down[key] = false;
	agent->player_movement(false);
}

void Input::check_input() {

	if (key_down['w'] || key_down['W']) {

		agent->player_movement(true, 'N');
	}
	if (key_down['a'] || key_down['A']) {

		agent->flip(true);
		agent->player_movement(true, 'W');
	}
	if (key_down['s'] || key_down['S']) {

		agent->player_movement(true, 'S');
	}
	if (key_down['d'] || key_down['D']) {

		agent->flip(false);
		agent->player_movement(true, 'E');
	}
	if (key_down[27]) {
	
		exit(0);
	}
}