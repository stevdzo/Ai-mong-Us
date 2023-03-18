#ifndef GameWorld_H
#define GameWorld_H
#pragma warning (disable:4786)

#include <vector>

#include "Vector2D.h" 
#include "BaseGameEntity.h"
#include "Agent.h"
#include "Background.h"
#include "Obstacle.h"
#include "StaticGameObject.h"

#include "Graph.h"

#include "Input.h"

class Path;

typedef std::vector<BaseGameEntity*>::iterator it;

class GameWorld {

private:

	std::vector<Agent*> agents;
	std::vector<Obstacle*> obstacles;

	Path* path;

	int screen_width;
	int screen_height;

	Vector2D crosshair;

	double timer = 0;

	Graph* graph;
	std::vector<GraphNode*> nodes;

	Input* input;
	Background* background;

	StaticGameObject* pin;

public:

	GameWorld(int, int);

	~GameWorld();

	void update(double);
	void render();

	void game_loop(double);

	void world_init();

	void init_graph();

	Graph* game_graph() { return graph; }

	const std::vector<Agent*>& get_agents() { return agents; }
	const std::vector<Obstacle*>& get_obstacles() { return obstacles; }

	Vector2D get_crosshair()const { return crosshair; }
	void set_crosshair(Vector2D v) { crosshair = v; }	

	Input* get_input() { return input; }

	void set_walls();

	void obstacle_nodes();

	bool game_over();
	bool game_won();
};
#endif