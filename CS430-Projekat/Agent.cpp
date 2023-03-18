#include <sstream>
#include <string>

#include "Agent.h"
#include "Matrix2D.h"
#include "SteeringBehaviors.h"
#include "GameWorld.h"
#include "State.h"
#include "AgentWander.h"
#include "Graph.h"
#include "Path.h"
#include "SOIL2.h"

using std::vector;
using std::list;

Agent::Agent(GameWorld* world,
    Vector2D  position,
    double    rotation,
    Vector2D  velocity,
    double    mass,
    double    max_force,
    double    max_speed,
    double    max_turn_rate,
    double    scale) : MovingEntity(position,
                                    scale,
                                    velocity,
                                    max_speed,
                                    Vector2D(sin(rotation), -cos(rotation)),
                                    mass,
                                    Vector2D(scale, scale),
                                    max_turn_rate,
                                    max_force),
                                    
                                    current_state(),
                                    game_world(world),
                                    smoothing_on(false),
                                    delta_time(0.0)
{
    initialize_buffer();

    steering_behavior = new SteeringBehavior(this);

    graph = game_world->game_graph();

    astar = new AStar(graph, find_node(), graph->get_nodes()[126]);

    animation = idle;
   
    walk_animation = new GLuint[48];
    idle_animation = new GLuint[1];
    death_animation = new GLuint[42];
    agent_dead_animation = new GLuint[48];
    agent_dead_idle_animation = new GLuint[1];

    walk_texture_index = 0;
    idle_texture_index = 0;
    death_texture_index = 0;
    agent_dead_texture_index = 0;
    agent_dead_idle_texture_index = 0;

    texture_index = 0;
    current_frame = 0;
    number_of_frames = 1;
    animation_delay = 0.05f;
    animation_elapsed = 0.0f;

    size.width = 165.0f/3;
    size.height = 211.0f/3;

    sheet = false;
    flip_horizontal = false;

    active = true;
    visible = true;
    transparent = false;
    player = false;
    cannot_walk = false;
    animation_looped = true;
    hat_picked_up = false;

    pin = new StaticGameObject();
    pin->add_texture("resources/mixed/pin.png", true);
    pin->set_size(471/14, 717/14);
    pin->is_visible(false);

    mark = new StaticGameObject(false);
    mark->is_visible(false);

    dead_body = new StaticGameObject();
    dead_body->add_texture("resources/death/Dead0042.png", true);
    dead_body->set_size(178/3, 137/3);
    dead_body->is_visible(false);

    hat = new StaticGameObject();
    hat->add_texture("resources/mixed/hat.png", true);
    hat->set_size(178 / 3, 173 / 3);
    hat->set_position(Vector2D(740, 710));
    hat->is_visible(true);
   
}

Agent::~Agent() {

    delete steering_behavior;
    delete current_state;
    delete pin;
    delete mark;
    delete walk_animation;
    delete idle_animation;
    delete death_animation;
    delete agent_dead_animation;
    delete astar;
}

void Agent::update(double time_elapsed) {

    if (active) {

        animation_elapsed += time_elapsed;
        if (animation_elapsed >= animation_delay) {

            current_frame++;
            if (current_frame >= number_of_frames) { current_frame = 0; }

            animation_elapsed = 0.0f;
        }

        if (current_state) current_state->execute(this);

        delta_time = time_elapsed;

        Vector2D old_position = get_position();

        Vector2D steering_force;

        steering_force = steering_behavior->calculate();

        Vector2D acceleration = steering_force / mass;

        velocity += acceleration * time_elapsed;

        velocity.truncate(max_speed);

        previous_position = position;

        position += velocity * time_elapsed;

        if (velocity.magnitude_sq() > 0.00000001) {

            heading = normalize_2d(velocity);

            side = heading.perp();
        }
        if (player) { player_boundaries(position, velocity, bounding_radius, SCREEN_WIDTH, SCREEN_HEIGHT); }
        else boundaries(position, velocity, bounding_radius, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

void Agent::render() { 
    
    //graph->display_path(astar->get_path());

    if (player) {
        if (hat_picked_up) { hat->set_position(Vector2D(this->position.x, this->position.y - 35)); }
        hat->render();
    }

    /*float angle = 0;
    glBegin(GL_LINE_LOOP);
    glColor3f(0.4, 0.0, 0.9);
    for (size_t i = 0; i < 360; i++) {

        angle = 2 * PI * i / 360;
        glVertex2f(position.x + bounding_radius * cos(angle), position.y + bounding_radius * sin(angle));
    }
    glEnd();
    glColor3f(1, 1, 1);*/

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glDisable(GL_DEPTH_TEST);

    if (visible) {
        if (transparent) {

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        if (animation == walking) { glBindTexture(GL_TEXTURE_2D, walk_animation[current_frame]); }
        else if (animation == death) { glBindTexture(GL_TEXTURE_2D, death_animation[current_frame]); }
        else if (animation == ghost) { glBindTexture(GL_TEXTURE_2D, agent_dead_animation[current_frame]); }
        else if (animation == dead_idle) { glBindTexture(GL_TEXTURE_2D, agent_dead_idle_animation[current_frame]); }
        else                               glBindTexture(GL_TEXTURE_2D, idle_animation[current_frame]);

        glBegin(GL_QUADS);

        if (flip_horizontal) {

            glTexCoord2f(1, 0);  glVertex2f(position.x - size.width/2, position.y - size.height/2);
            glTexCoord2f(0, 0);  glVertex2f(position.x + size.width/2, position.y - size.height/2);
            glTexCoord2f(0, 1);  glVertex2f(position.x + size.width/2, position.y + size.height/2);
            glTexCoord2f(1, 1);  glVertex2f(position.x - size.width/2, position.y + size.height/2);

        } else{

            glTexCoord2f(0, 0);  glVertex2f(position.x - size.width / 2, position.y - size.height / 2);
            glTexCoord2f(1, 0);  glVertex2f(position.x + size.width / 2, position.y - size.height / 2);
            glTexCoord2f(1, 1);  glVertex2f(position.x + size.width / 2, position.y + size.height / 2);
            glTexCoord2f(0, 1);  glVertex2f(position.x - size.width / 2, position.y + size.height / 2);
        }
        glEnd();

        if (transparent) { glDisable(GL_BLEND); }        
    }
    glDisable(GL_TEXTURE_2D);   

    if (pin->is_visible()) pin->render();
    if (player) mark->is_visible(true);
    if (mark->is_visible()) mark->render();
    mark->set_position(Vector2D(position.x, position.y - size.height/1.5));
    dead_body->render();   
}

bool Agent::add_walk_texture(const char* file_name, const bool _transparent) {

    GLuint texture = SOIL_load_OGL_texture(file_name, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    if (texture == 0) { return false; }

    walk_animation[walk_texture_index] = texture;
    walk_texture_index++;

    transparent = true;

    return true;
}

bool Agent::add_idle_texture(const char* file_name, const bool _transparent) {

    GLuint texture = SOIL_load_OGL_texture(file_name, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    if (texture == 0) { return false; }

    idle_animation[idle_texture_index] = texture;
    idle_texture_index++;

    transparent = true;

    return true;
}

bool Agent::add_death_texture(const char* file_name, const bool _transparent) {

    GLuint texture = SOIL_load_OGL_texture(file_name, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    if (texture == 0) { return false; }

    death_animation[death_texture_index] = texture;
    death_texture_index++;

    transparent = true;

    return true;
}

bool Agent::add_agent_dead_texture(const char* file_name, const bool _transparent) {

    GLuint texture = SOIL_load_OGL_texture(file_name, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    if (texture == 0) { return false; }

    agent_dead_animation[agent_dead_texture_index] = texture;
    agent_dead_texture_index++;

    transparent = true;

    return true;
}

bool Agent::add_agent_dead_idle_texture(const char* file_name, const bool _transparent) {

    GLuint texture = SOIL_load_OGL_texture(file_name, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    if (texture == 0) { return false; }

    agent_dead_idle_animation[agent_dead_idle_texture_index] = texture;
    agent_dead_idle_texture_index++;

    transparent = true;

    return true;
}

void Agent::player_movement(bool active, char direction) {

    if (active && !dead) {
    
        start_walk_animation();

        switch (direction) {
        
        case 'N':
            velocity.y = -MAX_PLAYER_VELOCITY;
            break;
        case 'W':
            velocity.x = -MAX_PLAYER_VELOCITY;
            break;
        case 'S':
            velocity.y = MAX_PLAYER_VELOCITY;
            break;
        case 'E':
            velocity.x = MAX_PLAYER_VELOCITY;
            break;
        }
    } else {

        velocity.x = 0;
        velocity.y = 0;       
    }
}

void Agent::render_forces() {

       /* glLineWidth(3.0f);
        glBegin(GL_LINES);
            glColor3f(0.0, 1.0, 0.0);
            glVertex2f(position.x, position.y);
            glVertex2f(target->get_position().x, target->get_position().y);
        glEnd();

        glBegin(GL_LINES);
            glColor3f(0.0, 1.0, 1.0);
            glVertex2f(position.x, position.y);
            glVertex2f(steering_force.x + position.x, steering_force.y + position.y);
        glEnd();

        glBegin(GL_LINES);
            glColor3f(1.0, 1.0, 0.0);
            glVertex2f(position.x, position.y);
            Vector2D heading = direction * 100;
            heading += position;
            glVertex2f(heading.x, heading.y);
        glEnd();

        glBegin(GL_LINES);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(position.x, position.y);
            Vector2D sidee = side * 100;
            sidee += position;
            glVertex2f(sidee.x, sidee.y);
        glEnd();*/
}

void Agent::initialize_buffer() {

    const int num_vehicle_verts = 3;

    Vector2D vehicle[num_vehicle_verts] = { Vector2D(-1.0f,0.6f),
                                            Vector2D(1.0f,0.0f),
                                            Vector2D(-1.0f,-0.6f) };

    for (int vtx = 0; vtx < num_vehicle_verts; ++vtx) {

        vec_vechicle_buff.push_back(vehicle[vtx]);
    }
}

void Agent::change_state(State* new_state) {

    if (current_state) current_state->exit(this);

    current_state = new_state;

    current_state->enter(this);
}

void Agent::hat_click(double x, double y) {

    if ((x < hat->get_position().x + hat->get_size().width &&
        x > hat->get_position().x - hat->get_size().width) &&
        (y < hat->get_position().y + hat->get_size().height &&
            y > hat->get_position().y - hat->get_size().height)) {
    
        if (vec_distance(this->position, hat->get_position()) < 80) {

            if (hat != nullptr) {
            
                hat_picked_up = true;
            }
        }
    }
}

Agent* Agent::find_attacked_agent(double x, double y) {

    auto agents = game_world->get_agents();

    for (auto& _agent : agents) {

        if ((x < _agent->get_position().x + _agent->size.width &&
             x > _agent->get_position().x - _agent->size.width) &&
            (y < _agent->get_position().y + _agent->size.height &&
             y > _agent->get_position().y - _agent->size.height)) {

            if (vec_distance(this->position, _agent->position) < 150) {

               // std::cout << _agent->get_id();
                return _agent;
            }
        }
    }
    return nullptr;
}

void Agent::player_attack(double x, double y) { 

    Agent* victim = find_attacked_agent(x, y);

    if (victim != nullptr && !victim->player && !victim->dead) {
           
        victim->dead = true;
        victim->dead_body->set_position(Vector2D(victim->get_position().x, victim->get_position().y));
        victim->dead_body->is_visible(true);
        // victim->steering()->obstacle_avoidance_off();
        victim->steering()->pursuit_off();
    }
}

void Agent::check_bot_animation() { // proverava se animacija na osnovu parametara, da li se krece, da li je ziv...

    if (abs(velocity.x) > 0.01) { flip(false); }
    if (velocity.x == 0 && velocity.y == 0) { start_idle_animation(); if (dead)  start_agent_dead_animation(); }
    else if (velocity.x > 0.1) {

        if (!dead) start_walk_animation();
        if (dead)  start_agent_dead_animation();
        flip(false);
        if (hat_picked_up) hat->flip(false);
    }
    else if (velocity.x < 0.1) { 

        if (!dead) start_walk_animation(); 
        if (dead)  start_agent_dead_animation();       
        flip(true); 
        if (hat_picked_up) hat->flip(true);
    }
}

void Agent::check_obstacle_collision(Obstacle * o) { // pri dodiru sa preprekom, agent se vraca na poslednju poziciju pre kontakta
  
    if (!dead) {

        if (vec_distance(position, o->get_position()) <
            bounding_radius + o->get_bounding_radius()) {

            if (player)position = previous_position;
            else { velocity.x = -velocity.x; velocity.y = -velocity.y; }
        }
    }
}

void Agent::load_textures() {

    for (int i = 1; i <= 12; i++) {

        std::string file_name;
        std::stringstream index;
        index << i;

        if (i < 10) file_name = "resources/walk/Walk000" + index.str() + ".png";
        else        file_name = "resources/walk/Walk00" + index.str() + ".png";

        add_walk_texture((char*)file_name.c_str());
    }

    for (int i = 1; i <= 48; i++) {

        std::string file_name;
        std::stringstream index;
        index << i;

        if (i < 10) file_name = "resources/ghost/ghost000" + index.str() + ".png";
        else        file_name = "resources/ghost/ghost00" + index.str() + ".png";

        add_agent_dead_texture((char*)file_name.c_str());
    }

    for (int i = 1; i <= 42; i++) {

        std::string file_name;
        std::stringstream index;
        index << i;

        if (i < 10) file_name = "resources/death/Dead000" + index.str() + ".png";
        else        file_name = "resources/death/Dead00" + index.str() + ".png";

        add_death_texture((char*)file_name.c_str());
    }
    add_idle_texture("resources/idle/idle.png");
    add_agent_dead_idle_texture("resources/death/Dead0042.png");
}

GraphNode* Agent::find_node() {

    auto nodes = game_world->game_graph()->get_nodes();
    GraphNode* temp = new GraphNode();

    for (auto& node : nodes) {
        
        if (node->get_position().x == (int)((position.x / 100) * 2) &&
            node->get_position().y == (int)((position.y / 100) * 2))
            temp = node;
    }
    return temp;
}

bool Agent::find_path(int x, int y) {

    if (!astar->get_path().empty()) 
        astar->get_path().clear();
    
    GraphNode* temp = graph->node_clicked(x, y);
    
    if (temp != nullptr && !temp->is_obstacle()) {

        pin->set_position(Vector2D(temp->get_position().x/2 * 100 + NODE_WIDTH/2, temp->get_position().y/2 * 100));
        pin->is_visible(true);
        astar->set_start_node(this->find_node());
        astar->set_target_node(temp);
        set_path(astar->find_shortest_path());

        return true;
    }
    return false;
}

std::vector<Vector2D> Agent::shortest_path() {

    return path->get_path();
}

Path* Agent::shortest_path2() {

    return path;
}

void Agent::set_path(Path* path) { steering_behavior->set_path(path); }

bool Agent::player_spotted() {

    if (vec_distance(position, steering()->get_target_agent_1()->get_position()) < 200) {

        return true;
    }
    return false;
}

bool Agent::player_disappear() {

    if (vec_distance(position, steering()->get_target_agent_1()->get_position()) > 100) {

        return true;
    }
    return false;
}

bool Agent::near_exit() {

    if (position.x < 50) {
    
        if (position.y + size.height / 2 < 540 && position.y > 440) { return true; }       
    }
    return false;
}