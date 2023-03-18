#ifndef AGENT_H
#define AGENT_H
#pragma warning (disable:4786)

#include "MovingEntity.h"
#include "Vector2D.h"
#include "Algorithms.h"
//#include "SteeringBehaviors.h"
#include "Obstacle.h"
#include "StaticGameObject.h"

#include <vector>
#include <list>
#include <string>

class GameWorld;
class SteeringBehavior;
class State;
class Graph;
class Path;

enum bot_type {

    pursuer = 1,
    evader = 2,
    path_follower = 3
};

enum enum_animation {

    walking = 1,
    idle = 2,
    death = 3,
    ghost = 4,
    dead_idle = 5
};

class Agent : public MovingEntity {

private:

	GameWorld* game_world;
	SteeringBehavior* steering_behavior;

    Agent(const Agent&);
    Agent& operator=(const Agent&);

    Vector2D smoothed_heading;
    bool smoothing_on;

    double delta_time;

    std::vector<Vector2D> vec_vechicle_buff;

    void initialize_buffer();

    double r, g, b;
   
    enum_animation animation;
    Graph* graph;
    AStar* astar;
    Path* path;

    bool killing;
    bool cannot_walk;
    bool hat_picked_up;

    Vector2D previous_position;

    StaticGameObject* pin;
    StaticGameObject* mark;
    StaticGameObject* dead_body;
    StaticGameObject* hat;

    Vector2D dead_body_position;

public:

    Agent(GameWorld* world,
            Vector2D  position,
            double    rotation,
            Vector2D  velocity,
            double    mass,
            double    max_force,
            double    max_speed,
            double    max_turn_rate,
            double    scale);

    ~Agent();

    void update(double);

    void render();
    void render_forces();

    bool add_texture(const char*, const bool = true) { return false; }
    bool add_walk_texture(const char*, const bool=true);
    bool add_idle_texture(const char*, const bool=true);
    bool add_death_texture(const char*, const bool=true);
    bool add_agent_dead_texture(const char*, const bool = true);
    bool add_agent_dead_idle_texture(const char*, const bool = true);

    bool is_killing() { return killing; }
    void is_killing(bool k) { killing = k; }

    const GLuint get_current_frame() { return sheet ? idle_animation[0] : textures[current_frame]; }

    bool is_player() { return player; }
    void is_player(bool p) { player = p; }

    bool set_cannot_walk() { return cannot_walk; }
    void set_cannot_walk(bool p) { cannot_walk = p; }

    void player_movement(bool, char=' ');

    void flip(bool b) { flip_horizontal = b; }

    State* current_state;

    void change_state(State*);

    void set_color(double r, double g, double b) { this->r = r; this->g = g, this->b = b; }

    SteeringBehavior* const steering()const { return steering_behavior; }
    GameWorld* const world()const { return game_world; }

    Vector2D get_smoothed_heading()const { return smoothed_heading; }

    bool is_smoothing_on()const { return smoothing_on; }
    void set_smoothing_on() { smoothing_on = true; }
    void set_smoothing_off() { smoothing_on = false; }
    void toggle_smoothing() { smoothing_on = !smoothing_on; }

    double time_elapsed()const { return delta_time; }

    bool agents_touched(Vector2D);
    void switch_velocity(Agent*);

    void start_walk_animation() { animation = walking; number_of_frames = 12; }
    void start_idle_animation() { animation = idle; number_of_frames = 1; }
    void start_death_animation() { animation = death; number_of_frames = 48; }
    void start_agent_dead_idle_animation() { animation = dead_idle; number_of_frames = 1; }
    void start_agent_dead_animation() { animation = ghost; number_of_frames = 48; }
    
    Agent* find_attacked_agent(double, double);
    void player_attack(double, double);
    void check_bot_animation();

    void check_obstacle_collision(Obstacle*);

    void load_textures();

    GraphNode* find_node();

    bool find_path(int, int);
    std::vector<Vector2D> shortest_path();
    Path* shortest_path2();

    void set_path(Path* path);

    void hat_click(double x, double y);
    void take_hat() { hat_picked_up = true; }
   
    bool is_hat_picked_up() { return hat_picked_up; }

    StaticGameObject* get_pin() { return pin; }
    void set_pin(StaticGameObject* p) { pin = p; }

    StaticGameObject* get_dead_body() { return dead_body; }
    void set_dead_body(StaticGameObject* d) { dead_body = d; }

    StaticGameObject* get_hat() { return hat; }
    void set_hat(StaticGameObject* h) { hat = h; }

    bool player_spotted();
    bool player_disappear();

    bot_type type;

    bool near_exit();
};
#endif