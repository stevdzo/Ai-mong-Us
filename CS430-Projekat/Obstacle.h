#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Vector2D.h"
#include "BaseGameEntity.h"

class Obstacle : public BaseGameEntity {

public:

	Obstacle(double x, double y, double r);

	Obstacle(Vector2D pos, double radius);

	virtual ~Obstacle();

	void update(double);
	void render();

	bool add_texture(const char*, const bool) { return false; }

	GraphNode* find_node() { return nullptr; }
};
#endif