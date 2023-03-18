#ifndef STATICGAMEOBJECT_H
#define STATICGAMEOBJECT_H

#include "BaseGameEntity.h"
#include "Graph.h"

class StaticGameObject : public BaseGameEntity {

public:

	StaticGameObject(bool=true);
	~StaticGameObject() {}

	void update(double);

	void render();

	bool add_texture(const char*, const bool = false);
	void flip(bool b) { flip_horizontal = b; }

	GraphNode* find_node() { return nullptr; }

	bool has_texture;

	bool is_visible() { return visible; }
	void is_visible(bool v) { visible = v; }
};
#endif