#ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H

#include "Globals.h"
#include <vector>
#include <string>
#include "Vector2D.h"
#include "Matrix2D.h"
#include "Utils.h"

#include "Graph.h"

struct Telegram;

class BaseGameEntity {

public:

	enum { default_entity_type = -1 };

	struct Rect {

		GLfloat top;
		GLfloat bottom;
		GLfloat left;
		GLfloat right;
	};

private:

	int id;
	int entity_type;
	bool tag;

	int valid_id() { static int next_id = 0; return next_id++; }

protected:

	Vector2D position;
	Vector2D scale;
	double bounding_radius;

	Size size;

	GLuint* textures;
	unsigned int texture_index;
	unsigned int current_frame;
	unsigned int number_of_frames;
	GLfloat animation_delay;
	GLfloat animation_elapsed;

	bool flip_horizontal;
	bool visible;
	bool active;
	bool sheet;
	bool transparent;
	bool player;
	bool animation_looped;

	unsigned int walk_texture_index;
	unsigned int idle_texture_index;
	unsigned int death_texture_index;
	unsigned int agent_dead_idle_texture_index;
	unsigned int agent_dead_texture_index;

	GLuint* walk_animation;
	GLuint* idle_animation;
	GLuint* death_animation;	
	GLuint* agent_dead_idle_animation;
	GLuint* agent_dead_animation;	

	Rect collision;

	BaseGameEntity() : id(valid_id()),
					   bounding_radius(0.0),
					   position(Vector2D()),
					   scale(Vector2D(1.0, 1.0)),
					   entity_type(default_entity_type),
					   tag(false)
	{
		collision.left = 0.0f;
		collision.right = 0.0f;
		collision.top = 0.0f;
		collision.bottom = 0.0f;
	}

	BaseGameEntity(int entity_type) : id(valid_id()),
									   bounding_radius(0.0),
									   position(Vector2D()),
									   scale(Vector2D(1.0, 1.0)),
									   entity_type(entity_type),
									   tag(false)
	{
		collision.left = 0.0f;
		collision.right = 0.0f;
		collision.top = 0.0f;
		collision.bottom = 0.0f;
	}

	BaseGameEntity(int entity_type, Vector2D pos, double r) : position(pos),
															  bounding_radius(r),
															  id(valid_id()),
															  scale(Vector2D(1.0, 1.0)),
															  entity_type(entity_type),
															  tag(false)

	{
		collision.left = 0.0f;
		collision.right = 0.0f;
		collision.top = 0.0f;
		collision.bottom = 0.0f;
	}

	BaseGameEntity(int entity_type, int forced_id) : id(forced_id),
													 bounding_radius(0.0),
													 position(Vector2D()),
													 scale(Vector2D(1.0, 1.0)),
													 entity_type(entity_type),
													 tag(false)
	{}

public: 

	virtual ~BaseGameEntity() {}

	virtual void update(double) {}

	virtual void render() {}

	virtual bool add_texture(const char*, const bool) = 0;
	
	virtual GraphNode* find_node() = 0;

	virtual bool handle_message(const Telegram&) { return false; }

	virtual void write(std::ostream& os) const {}
	virtual void read(std::ifstream& is) {}

	int get_id() const { return id; }

	int get_entity_type() const { return entity_type; }
	void set_entity_type(int new_type) { entity_type = new_type; }

	Vector2D get_position() const { return position; }
	void set_position(Vector2D new_position) { position = new_position; }

	double get_bounding_radius() { return bounding_radius; }
	void set_bounding_radius(double r) { bounding_radius = r; }

	bool is_tagged() { return tag; }
	void set_tag() { tag = true; }
	void untag() { tag = false; }

	Vector2D get_scale() const { return scale; }
	void set_scale(Vector2D new_scale) { scale = new_scale; }
	void set_scale(double val) { bounding_radius *= (val / max_of(scale.x, scale.y)); scale = Vector2D(val, val); }	

	void set_size(float w, float h) {
	
		size.width = w;
		size.height = h;
	}

	Size get_size() { return size; }

	const Rect get_collision_rect() const {
		Rect rect;
		rect.left = position.x + collision.left;
		rect.right = position.x + size.width + collision.right;
		rect.top = position.y + collision.top;
		rect.bottom = position.y + size.height + collision.bottom;
		return rect;
	}
};
#endif