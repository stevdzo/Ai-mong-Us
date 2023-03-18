#ifndef INPUT_H
#define INPUT_H

#include "Algorithms.h"
#include "Agent.h"

class GameWorld;

class Input {
	
private:

	bool key_down[256];

	Agent* agent;
	Graph* graph;
	AStar astar;

public:

	Input();
	Input(Agent* _agent, Graph* _graph);
	Input(Agent* _agent);
	~Input();

	void mouse(int, int, int, int);
	void keyboard_press(unsigned char key, int x, int y);
	void keyboard_up(unsigned char key, int x, int y);
	void check_input();
};
#endif