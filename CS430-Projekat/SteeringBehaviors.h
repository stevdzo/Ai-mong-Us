#ifndef STEERING_BEHAVIORS_H
#define STEERING_BEHAVIORS_H
#pragma warning (disable:4786)

#include <vector>
#include <windows.h>
#include <string>
#include <list>

#include "Vector2D.h"
#include "Path.h"
#include "Obstacle.h"
//#include "Wall.h"

class Agent;
class BaseGameEntity;

const double wander_rad = 15;
const double wander_dist = 20.0;
const double wander_jitter_per_sec = 800.0;
const double waypoint_seek_dist = 10;

class SteeringBehavior {

private:		

	enum behavior {

		bnone = 0x00000,
		bseek = 0x00002,
		bflee = 0x00004,
		barrive = 0x00008,
		bwander = 0x0010,
		bfollow_path = 0x00020,
		bpursuit = 0x00040,
		bevade = 0x00080,
		boffset_pursuit = 0x00100,
		bobstacle_avoidance = 0x00200,
		bwall_avoidance = 0x00400
	};

	Agent* agent;

	Vector2D steering_force;

	Agent* target_agent_1;
	Agent* target_agent_2;

	Vector2D target;

	double box_length;

	Vector2D wander_target;

	double wander_jitter;
	double wander_radius;
	double wander_distance;

	double view_distance;

	Path* path;

	double waypoint_seek_distance_sq;

	Vector2D offset;

	int flags;

	enum Deceleration { slow = 3, normal = 2, fast = 1 };

	Deceleration deceleration;


	bool on(behavior b) { return (flags & b) == b; }

	bool accumulate_force(Vector2D& sf, Vector2D force_to_add);

	Vector2D seek(Vector2D);
	Vector2D flee(Vector2D);
	Vector2D arrive(Vector2D, Deceleration);
	Vector2D pursuit(const Agent* agent);
	Vector2D offset_pursuit(const Agent*, const Vector2D);
	Vector2D evade(const Agent*);
	Vector2D wander();
	Vector2D follow_path();
	//Vector2D obstacle_avoidance(const std::vector<Obstacle*>&);
    Vector2D obstacle_avoidance();
    Vector2D wall_avoidance();

public:

	SteeringBehavior(Agent*);

	virtual ~SteeringBehavior();

	Vector2D calculate();

	void set_target(const Vector2D t) { target = t; }

	void set_target_agent_1(Agent* a) { target_agent_1 = a; }
	void set_target_agent_2(Agent* a) { target_agent_2 = a; }

	Agent* get_target_agent_1() { return target_agent_1; }
	Agent* get_target_agent_2() { return target_agent_2; }

	void set_path(Path* p) { path = p; std::cout << path->get_path().size() << std::endl; }

	void seek_on() { flags |= bseek; }
	void flee_on() { flags |= bflee; }
	void arrive_on() { flags |= barrive; }
	void pursuit_on(Agent* a) { flags |= bpursuit; target_agent_1 = a; }
	void offset_pursuit_on(Agent* a, const Vector2D off) { flags |= boffset_pursuit; offset = off; target_agent_1 = a; }
	void evade_on(Agent* a) { flags |= bevade, target_agent_1 = a; }
	void wander_on() { flags |= bwander; }
	void follow_path_on() { flags |= bfollow_path; }
	void obstacle_avoidance_on() { flags |= bobstacle_avoidance; }
	void wall_avoidance_on() { flags |= bwall_avoidance; }

	void seek_off() { if (on(bseek)) flags ^= bseek; }
	void flee_off() { if (on(bflee)) flags ^= bflee; }
	void arrive_off() { if (on(barrive)) flags ^= barrive; }
	void pursuit_off() { if (on(bpursuit)) flags ^= bpursuit; }
	void offset_pursuit_off() { if (on(boffset_pursuit)) flags ^= boffset_pursuit; }
	void evade_off() { if (on(bevade)) flags ^= bevade; }
	void wander_off() { if (on(bwander)) flags ^= bwander; }
	void follow_path_off() { if (on(bfollow_path)) flags ^= bfollow_path; }
	void obstacle_avoidance_off() { flags ^= bobstacle_avoidance; }
	void wall_avoidance_off() { flags ^= bwall_avoidance; }

	bool is_seek_on() { return on(bseek); }
	bool is_flee_on() { return on(bflee); }
	bool is_arrive_on() { return on(barrive); }
	bool is_pursuit_on() { return on(bpursuit); }
	bool is_offset_pursuit_on() { return on(boffset_pursuit); }
	bool is_evade_on() { return on(bevade); }
	bool is_wander_on() { return on(bwander); }
	bool is_follow_path_on() { return on(bfollow_path); }
	bool is_obstacle_avoidance_on() { return on(bobstacle_avoidance); }
	bool is_wall_avoidance_on() { return on(bwall_avoidance); }

	double get_wander_jitter()const { return wander_jitter; }
	double get_wander_distance()const { return wander_distance; }
	double get_wander_radius()const { return wander_radius; }
};
#endif