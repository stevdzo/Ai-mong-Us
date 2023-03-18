#ifndef BASEGAMEENTITY_H
#define BASEGAMEENTITY_H

#include "BaseGameEntity.h"
#include "Graph.h"

class Background : public BaseGameEntity {

public:

	Background();

	~Background() {}

	void update(double) {}

	void render();

	bool add_texture(const char*, const bool=false);

	GraphNode* find_node() { return nullptr; }

};
#endif