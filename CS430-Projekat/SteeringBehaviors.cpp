#include "SteeringBehaviors.h"
#include "Agent.h"
#include "Transformations.h"
#include "Utils.h"
#include "GameWorld.h"
#include "BaseGameEntity.h"

#include <cassert>

using std::string;
using std::vector;

SteeringBehavior::SteeringBehavior(Agent* agent) : agent(agent),
												   flags(0),
												   box_length(40.0), 
												   view_distance(50.0),
												   deceleration(fast), 
												   target_agent_1(NULL), 
												   target_agent_2(NULL), 
												   wander_distance(wander_dist),
												   wander_jitter(wander_jitter_per_sec), 
												   wander_radius(wander_rad),
												   waypoint_seek_distance_sq(waypoint_seek_dist*waypoint_seek_dist)
{
	double theta = rand_float() * two_PI;
	wander_target = Vector2D(wander_radius * cos(theta), wander_radius * sin(theta));
	
	path = new Path();

	srand(time(NULL));
}

SteeringBehavior::~SteeringBehavior() { delete path; }

Vector2D SteeringBehavior::calculate() {

	steering_force.zero();

	if (on(bevade)) {

		steering_force += evade(target_agent_1);
	}

	if (on(bseek)) {
		steering_force += seek(agent->world()->get_crosshair());
	}

	if (on(bflee)) {
		steering_force += flee(agent->world()->get_crosshair());
	}

	if (on(barrive)) {
		steering_force += arrive(target_agent_1->get_position(), deceleration);
	}

	if (on(bwander)) {
		 
		steering_force += wander();
	}

	if (on(bpursuit)) {

		steering_force += pursuit(target_agent_1);
	}

	if (on(boffset_pursuit)) {

		steering_force += offset_pursuit(target_agent_1, offset);
	}

	if (on(bfollow_path)) {

		steering_force += follow_path();
	}

	if (on(bobstacle_avoidance)) {
	
		steering_force += obstacle_avoidance();
	}

	//steering_force.truncate(agent->get_max_force());

	return steering_force;
}

bool SteeringBehavior::accumulate_force(Vector2D& v, Vector2D force_to_add) {

	double magnitude_so_far = v.magnitude();
	double magnitude_remaining = agent->get_max_force() - magnitude_so_far;

	if (magnitude_remaining <= 0.0) return false;

	double magnitude_to_add = force_to_add.magnitude();

	if (magnitude_to_add < magnitude_remaining) {
	
		v += force_to_add;
	}
	else {
	
		v += (normalize_2d(force_to_add) * magnitude_remaining);
	}
	return true;
}

Vector2D SteeringBehavior::seek(Vector2D target) {

	/*if (target.distance_to_sq(agent->get_position()) < 10) {

		return arrive(target, fast);
	}*/

	Vector2D desired_velocity = normalize_2d(target - agent->get_position()) * agent->get_max_speed();

	return (desired_velocity - agent->get_velocity());
}

Vector2D SteeringBehavior::flee(Vector2D target) {

	const double panic_distance_sq = 100.0f * 100.0f;

	if (vec_distance_sq(agent->get_position(), target) > panic_distance_sq) {

		return Vector2D(0, 0);
	}

	Vector2D desired_velocity = normalize_2d(agent->get_position() - target) * agent->get_max_speed();

	return (desired_velocity - agent->get_velocity());
}

Vector2D SteeringBehavior::arrive(Vector2D target, Deceleration dec) {

	Vector2D to_target = target - agent->get_position();

	double dist = to_target.magnitude();

	if (dist > 10) {
	
		Vector2D desired_velocity = to_target * 10;

		return (desired_velocity - agent->get_velocity());
	}	
	if (agent->get_velocity().magnitude() < agent->get_max_speed()) {

		arrive_off();
		agent->set_velocity(Vector2D(0, 0));
	}

	return Vector2D(0, 0);
}

Vector2D SteeringBehavior::pursuit(const Agent* evader) {

	Vector2D to_evader = evader->get_position() - agent->get_position();

	double relative_heading = agent->get_heading().dot(evader->get_heading());

	if ((to_evader.dot(agent->get_heading()) > 0) && (relative_heading < -0.95)) {
	
		return seek(evader->get_position());
	}
	double look_ahead_time = to_evader.magnitude() / (agent->get_max_speed() + evader->speed());

	return seek(evader->get_position() + evader->get_velocity() * look_ahead_time);
}

Vector2D SteeringBehavior::offset_pursuit(const Agent*, const Vector2D) {

	return Vector2D();
}

Vector2D SteeringBehavior::evade(const Agent* pursuer) {

	Vector2D to_pursuer = pursuer->get_position() - agent->get_position();

	const double threat_range = 150.0;
	if (to_pursuer.magnitude_sq() > threat_range * threat_range) return Vector2D();

	double look_ahead_time = to_pursuer.magnitude() /
		(agent->get_max_speed() + pursuer->speed());

	return flee(pursuer->get_position() + pursuer->get_velocity() * look_ahead_time);
}

Vector2D SteeringBehavior::wander() {

	double jitter_this_time_slice = wander_jitter * agent->time_elapsed();

	wander_target += Vector2D(random_clamped() * jitter_this_time_slice, random_clamped() * jitter_this_time_slice);

	wander_target.normalize();

	wander_target *= wander_radius;

	Vector2D target = wander_target + Vector2D(wander_distance, 0);

	Vector2D world_space_target = point_to_world_space(target, agent->get_heading(), agent->get_side(), agent->get_position());

	return world_space_target - agent->get_position();
}

Vector2D SteeringBehavior::follow_path() {

	if (!path) { return Vector2D(); }

	if (vec_distance_sq(path->current_checkpoint(), agent->get_position()) < waypoint_seek_distance_sq) {

		if (agent->is_player() && !path->last_checkpoint())
		path->set_next_checkpoint();
		else if (!agent->is_player()) { path->set_next_checkpoint(); }
	}
	if (!path->last_checkpoint()) {

		return seek(path->current_checkpoint());
	}
	else { if (agent->is_player()) agent->get_pin()->is_visible(false); return arrive(path->current_checkpoint(), fast); }
}

Vector2D SteeringBehavior::obstacle_avoidance() {

	Vector2D temp_steering_force = Vector2D();

	double box_length = 100 + (agent->speed() / max_of(agent->get_max_speed(), .001)) * 100;

	std::vector<Obstacle*> all_obstacles = agent->world()->get_obstacles();

	auto iterator = all_obstacles.begin();
	Obstacle* o;
	Obstacle* closest_obstacle = nullptr;
	double closest_x = std::numeric_limits<double>::infinity();
	Vector2D local_pos_of_closest;

	auto velocity_normal = agent->get_velocity().vec_normalize();
	auto velocity_ortho = velocity_normal.perp();

	while (iterator != all_obstacles.end()) {

		o = *iterator++;

		if (o->get_position().distance_to_sq(agent->get_position() + Vector2D(20, 20)) > box_length* box_length) {

			continue;
		}

		Vector2D obs_local = point_to_local_space(o->get_position(), velocity_normal, velocity_ortho, agent->get_position());

		if (obs_local.x < 0) {

			continue;
		}
		if (abs(obs_local.y) - o->get_bounding_radius() > agent->get_bounding_radius()) {

			continue;
		}
		if (obs_local.x < closest_x) {

			closest_obstacle = o;
			local_pos_of_closest = obs_local;
			closest_x = obs_local.x;
		}
	}
	if (closest_obstacle) {

		double multiplier = .50 + (box_length - local_pos_of_closest.x) / box_length;

		temp_steering_force.y = (closest_obstacle->get_bounding_radius() - local_pos_of_closest.y) * multiplier;

		double braking_weight = 0.05;

		temp_steering_force.x = (closest_obstacle->get_bounding_radius() - local_pos_of_closest.x) * braking_weight;

		return vector_to_world_space(temp_steering_force, velocity_normal, velocity_ortho);
	}
	return Vector2D();
}

Vector2D SteeringBehavior::wall_avoidance() {



	return Vector2D();
}