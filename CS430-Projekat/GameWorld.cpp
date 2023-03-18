#include "GameWorld.h"
#include "Agent.h"
#include "Transformations.h"
#include "SteeringBehaviors.h"

#include "AgentEvade.h"
#include "AgentPursuit.h"
#include "AgentWander.h"
#include "AgentFollowPath.h"

#include "EntityManager.h"
#include "BotCommunication.h"

#include <list>
using std::list;

GameWorld::GameWorld(int x, int y) : screen_width(x),
									 screen_height(y),
									 crosshair(Vector2D(100, 500)),
									 path(NULL),
									 timer(0)
{
	graph = new Graph();

	init_graph();

	background = new Background();
	background->add_texture("resources/background/skeld.png");
	background->set_size(1000, 1000);

	pin = new StaticGameObject();
	pin->add_texture("resources/mixed/pin.png", true);

	Agent* agent = new Agent(this,
		Vector2D(140, 100),
		(rand_float() * two_PI),
		Vector2D(0, 0),
		0.1,
		20.0,
		150,
		PI,
		15.0);
	
	agent->is_player(true);

	agent->load_textures();

	agents.push_back(agent);

	agent = new Agent(this,
		Vector2D(860, 400),
		(rand_float() * two_PI),
		Vector2D(0, 0),
		0.1,
		2.0,
		110.0,
		PI,
		15.0);

	agent->load_textures();

	agent->change_state(AgentWander::instance());
	agent->steering()->obstacle_avoidance_on();
	agent->steering()->set_target_agent_1(agents[0]);	

	agent->type = bot_type::pursuer;

	entity_manager->add_entity(agent);
	dispatch->add_agent(agent);

	agents.push_back(agent);	

	agent = new Agent(this,
		Vector2D(850, 320),
		(rand_float() * two_PI),
		Vector2D(0, 0),
		0.1,
		50.0,
		100.0,
		PI,
		15.0);

	agent->load_textures();

	agent->change_state(AgentFollowPath::instance());
	agent->steering()->obstacle_avoidance_on();
	agent->steering()->set_target_agent_1(agents[0]);

	agent->type = bot_type::path_follower;

	entity_manager->add_entity(agent);
	dispatch->add_agent(agent);

	agents.push_back(agent);

	agent = new Agent(this,
		Vector2D(140, 450),
		(rand_float() * two_PI),
		Vector2D(0, 0),
		0.1,
		50.0,
		170.0,
		PI,
		15.0);

	agent->load_textures();

	agent->change_state(AgentEvade::instance());
	agent->steering()->obstacle_avoidance_on();
	agent->steering()->set_target_agent_1(agents[0]);

	agent->type = bot_type::evader;

	entity_manager->add_entity(agent);
	dispatch->add_agent(agent);

	agents.push_back(agent);

	Obstacle* obs = new Obstacle(262, 274, 100);
	obstacles.push_back(obs);

	obs = new Obstacle(689,279, 100);
	obstacles.push_back(obs);

	obs = new Obstacle(480, 499, 100);
	obstacles.push_back(obs);

	obs = new Obstacle(261, 716, 100);
	obstacles.push_back(obs);

	obs = new Obstacle(692, 718, 100);
	obstacles.push_back(obs);	

	set_walls();

	input = new Input(agents[0], graph);	
}

GameWorld::~GameWorld() {

	for (auto &i : agents) { delete i; }
	for (auto& i : obstacles) { delete i; }
	for (auto& i : obstacles) { delete i; }
	delete background;
	delete graph;
	delete input;
}

void GameWorld::update(double dt) {

	for (size_t i = 0; i < agents.size(); i++) { 

		agents[i]->update(dt);
		agents[i]->check_bot_animation();
	}
	input->check_input();

	for (auto& obstacle : obstacles) {

		for (auto & agent : agents) {

			agent->check_obstacle_collision(obstacle);
		}
	}
}

void GameWorld::render() {

	glColor3f(1, 1, 1);
	background->render();
	for (size_t i = 1; i < agents.size(); i++) {

		agents[i]->render();	
	}
	agents[0]->render();
	/*for (auto& obstacle : obstacles) {
		
		obstacle->render();
	}*/
}

void GameWorld::game_loop(double dt) {

	update(dt);
	render();

	if (game_over()) { 

		draw_text((char*)"GAME OVER", SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2); 
		agents[0]->is_dead(true);
	}

	if (game_won()) {

		draw_text((char*)"GAME WON", SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2);
		//agents[0]->is_dead(true);
	}

	//graph->render();
}

void GameWorld::init_graph() {

	GraphNode*** nodes_matrix = new GraphNode * *[ROWS];
	for (size_t i = 0; i < ROWS; i++) {

		nodes_matrix[i] = new GraphNode * [COLUMNS];
	}

	int index = 0;
	for (size_t rows = 0; rows < ROWS; rows++) {
		for (size_t columns = 0; columns < COLUMNS; columns++) {

			GraphNode* node = new GraphNode(index, Vector2D(rows, columns));

			index++;
			nodes.push_back(node);
			nodes_matrix[rows][columns] = node;
		}
	}
	graph->set_nodes(nodes);

	index = 0;
	for (size_t rows = 0; rows < ROWS; rows++) {
		for (size_t columns = 0; columns < COLUMNS; columns++) {
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i == 0 && j == 0) continue;
					if (rows + i >= 0 &&
						rows + i < ROWS &&
						columns + j >= 0 &&
						columns + j < COLUMNS) {

						graph->add_edge(GraphEdge(nodes_matrix[rows][columns], nodes_matrix[rows + i][columns + j]));
						graph->add_edge(GraphEdge(nodes_matrix[rows][columns], nodes_matrix[rows + i][columns + j]));
					}
				}
			}
		}
	}
	obstacle_nodes();
}

bool GameWorld::game_over() {

	for (int i = 1; i < agents.size(); i++) {

		if (vec_distance(agents[i]->get_position(), agents[i]->steering()->get_target_agent_1()->get_position()) < 10 && !agents[i]->is_dead()) {

			return true;
		}
	}
	return false;
}

bool GameWorld::game_won() {

	if (agents[0]->is_hat_picked_up() && agents[0]->near_exit()) {	
	
		return true;
	}
	return false;
}

// manuelno postavljene prepreke u sledece dve funkcije . . .

void GameWorld::obstacle_nodes() {

	// gornji levi ugao

	graph->get_nodes()[0]->is_obstacle(true);
	graph->get_nodes()[1]->is_obstacle(true);
	graph->get_nodes()[2]->is_obstacle(true);
	graph->get_nodes()[3]->is_obstacle(true);
	graph->get_nodes()[20]->is_obstacle(true);
	graph->get_nodes()[21]->is_obstacle(true);
	graph->get_nodes()[22]->is_obstacle(true);
	graph->get_nodes()[40]->is_obstacle(true);
	graph->get_nodes()[41]->is_obstacle(true);
	graph->get_nodes()[60]->is_obstacle(true);

	// gornji desni ugao

	graph->get_nodes()[300]->is_obstacle(true);
	graph->get_nodes()[320]->is_obstacle(true);
	graph->get_nodes()[321]->is_obstacle(true);
	graph->get_nodes()[340]->is_obstacle(true);
	graph->get_nodes()[341]->is_obstacle(true);
	graph->get_nodes()[342]->is_obstacle(true);
	graph->get_nodes()[360]->is_obstacle(true);
	graph->get_nodes()[361]->is_obstacle(true);
	graph->get_nodes()[362]->is_obstacle(true);
	graph->get_nodes()[363]->is_obstacle(true);
	graph->get_nodes()[380]->is_obstacle(true);
	graph->get_nodes()[381]->is_obstacle(true);
	graph->get_nodes()[382]->is_obstacle(true);
	graph->get_nodes()[383]->is_obstacle(true);
	graph->get_nodes()[384]->is_obstacle(true);

	// gornji levi sto

	graph->get_nodes()[64]->is_obstacle(true);
	graph->get_nodes()[65]->is_obstacle(true);
	graph->get_nodes()[66]->is_obstacle(true);
	graph->get_nodes()[83]->is_obstacle(true);
	graph->get_nodes()[84]->is_obstacle(true);
	graph->get_nodes()[85]->is_obstacle(true);
	graph->get_nodes()[86]->is_obstacle(true);
	graph->get_nodes()[87]->is_obstacle(true);
	graph->get_nodes()[103]->is_obstacle(true);
	graph->get_nodes()[104]->is_obstacle(true);
	graph->get_nodes()[105]->is_obstacle(true);
	graph->get_nodes()[106]->is_obstacle(true);
	graph->get_nodes()[107]->is_obstacle(true);
	graph->get_nodes()[123]->is_obstacle(true);
	graph->get_nodes()[124]->is_obstacle(true);
	graph->get_nodes()[125]->is_obstacle(true);
	graph->get_nodes()[126]->is_obstacle(true);
	graph->get_nodes()[127]->is_obstacle(true);
	graph->get_nodes()[144]->is_obstacle(true);
	graph->get_nodes()[145]->is_obstacle(true);
	graph->get_nodes()[146]->is_obstacle(true);

	// srednji sto

	graph->get_nodes()[149]->is_obstacle(true);
	graph->get_nodes()[150]->is_obstacle(true);
	graph->get_nodes()[151]->is_obstacle(true);
	graph->get_nodes()[168]->is_obstacle(true);
	graph->get_nodes()[169]->is_obstacle(true);
	graph->get_nodes()[170]->is_obstacle(true);
	graph->get_nodes()[171]->is_obstacle(true);
	graph->get_nodes()[188]->is_obstacle(true);
	graph->get_nodes()[189]->is_obstacle(true);
	graph->get_nodes()[190]->is_obstacle(true);
	graph->get_nodes()[191]->is_obstacle(true);
	graph->get_nodes()[208]->is_obstacle(true);
	graph->get_nodes()[209]->is_obstacle(true);
	graph->get_nodes()[210]->is_obstacle(true);
	graph->get_nodes()[211]->is_obstacle(true);
	graph->get_nodes()[229]->is_obstacle(true);
	graph->get_nodes()[230]->is_obstacle(true);
	graph->get_nodes()[231]->is_obstacle(true);

	// gornji desni sto

	graph->get_nodes()[224]->is_obstacle(true);
	graph->get_nodes()[225]->is_obstacle(true);
	graph->get_nodes()[226]->is_obstacle(true);
	graph->get_nodes()[244]->is_obstacle(true);
	graph->get_nodes()[245]->is_obstacle(true);
	graph->get_nodes()[246]->is_obstacle(true);
	graph->get_nodes()[247]->is_obstacle(true);
	graph->get_nodes()[263]->is_obstacle(true);
	graph->get_nodes()[264]->is_obstacle(true);
	graph->get_nodes()[265]->is_obstacle(true);
	graph->get_nodes()[266]->is_obstacle(true);
	graph->get_nodes()[267]->is_obstacle(true);
	graph->get_nodes()[283]->is_obstacle(true);
	graph->get_nodes()[284]->is_obstacle(true);
	graph->get_nodes()[285]->is_obstacle(true);
	graph->get_nodes()[286]->is_obstacle(true);
	graph->get_nodes()[287]->is_obstacle(true);
	graph->get_nodes()[304]->is_obstacle(true);
	graph->get_nodes()[305]->is_obstacle(true);
	graph->get_nodes()[306]->is_obstacle(true);

	// donji levi sto

	graph->get_nodes()[73]->is_obstacle(true);
	graph->get_nodes()[74]->is_obstacle(true);
	graph->get_nodes()[75]->is_obstacle(true);
	graph->get_nodes()[92]->is_obstacle(true);
	graph->get_nodes()[93]->is_obstacle(true);
	graph->get_nodes()[94]->is_obstacle(true);
	graph->get_nodes()[95]->is_obstacle(true);
	graph->get_nodes()[96]->is_obstacle(true);
	graph->get_nodes()[112]->is_obstacle(true);
	graph->get_nodes()[113]->is_obstacle(true);
	graph->get_nodes()[114]->is_obstacle(true);
	graph->get_nodes()[115]->is_obstacle(true);
	graph->get_nodes()[116]->is_obstacle(true);
	graph->get_nodes()[132]->is_obstacle(true);
	graph->get_nodes()[133]->is_obstacle(true);
	graph->get_nodes()[134]->is_obstacle(true);
	graph->get_nodes()[135]->is_obstacle(true);
	graph->get_nodes()[136]->is_obstacle(true);
	graph->get_nodes()[153]->is_obstacle(true);
	graph->get_nodes()[154]->is_obstacle(true);
	graph->get_nodes()[155]->is_obstacle(true);

	// donji desni sto

	graph->get_nodes()[233]->is_obstacle(true);
	graph->get_nodes()[234]->is_obstacle(true);
	graph->get_nodes()[235]->is_obstacle(true);
	graph->get_nodes()[252]->is_obstacle(true);
	graph->get_nodes()[253]->is_obstacle(true);
	graph->get_nodes()[254]->is_obstacle(true);
	graph->get_nodes()[255]->is_obstacle(true);
	graph->get_nodes()[272]->is_obstacle(true);
	graph->get_nodes()[273]->is_obstacle(true);
	graph->get_nodes()[274]->is_obstacle(true);
	graph->get_nodes()[275]->is_obstacle(true);
	graph->get_nodes()[276]->is_obstacle(true);
	graph->get_nodes()[292]->is_obstacle(true);
	graph->get_nodes()[293]->is_obstacle(true);
	graph->get_nodes()[294]->is_obstacle(true);
	graph->get_nodes()[295]->is_obstacle(true);
	graph->get_nodes()[296]->is_obstacle(true);
	graph->get_nodes()[313]->is_obstacle(true);
	graph->get_nodes()[314]->is_obstacle(true);
	graph->get_nodes()[315]->is_obstacle(true);

	// donji desni levi 

	graph->get_nodes()[15]->is_obstacle(true);
	graph->get_nodes()[16]->is_obstacle(true);
	graph->get_nodes()[17]->is_obstacle(true);
	graph->get_nodes()[18]->is_obstacle(true);
	graph->get_nodes()[19]->is_obstacle(true);
	graph->get_nodes()[36]->is_obstacle(true);
	graph->get_nodes()[37]->is_obstacle(true);
	graph->get_nodes()[38]->is_obstacle(true);
	graph->get_nodes()[39]->is_obstacle(true);
	graph->get_nodes()[57]->is_obstacle(true);
	graph->get_nodes()[58]->is_obstacle(true);
	graph->get_nodes()[59]->is_obstacle(true);
	graph->get_nodes()[78]->is_obstacle(true);
	graph->get_nodes()[79]->is_obstacle(true);
	graph->get_nodes()[99]->is_obstacle(true);

	// donji desni ugao

	graph->get_nodes()[318]->is_obstacle(true);
	graph->get_nodes()[319]->is_obstacle(true);
	graph->get_nodes()[337]->is_obstacle(true);
	graph->get_nodes()[338]->is_obstacle(true);
	graph->get_nodes()[339]->is_obstacle(true);
	graph->get_nodes()[356]->is_obstacle(true);
	graph->get_nodes()[357]->is_obstacle(true);
	graph->get_nodes()[358]->is_obstacle(true);
	graph->get_nodes()[359]->is_obstacle(true);
	graph->get_nodes()[376]->is_obstacle(true);
	graph->get_nodes()[377]->is_obstacle(true);
	graph->get_nodes()[378]->is_obstacle(true);
	graph->get_nodes()[379]->is_obstacle(true);
	graph->get_nodes()[395]->is_obstacle(true);
	graph->get_nodes()[396]->is_obstacle(true);
	graph->get_nodes()[397]->is_obstacle(true);
	graph->get_nodes()[398]->is_obstacle(true);
	graph->get_nodes()[399]->is_obstacle(true);
}

void GameWorld::set_walls() {

	// desna gornja dijagonala

	Obstacle* obs = new Obstacle(840, 20, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(870, 50, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(900, 80, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(930, 110, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(960, 130, 80);
	obstacles.push_back(obs);

	// leva gornja dijagonala
	
	obs = new Obstacle(80, 20, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(50, 50, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(20, 80, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(-10, 110, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(-40, 130, 80);
	obstacles.push_back(obs);

	// leva donja dijagonala

	obs = new Obstacle(130, 980, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(100, 950, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(70, 920, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(40, 890, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(10, 860, 80);
	obstacles.push_back(obs);

	// desna donja dijagonala

	obs = new Obstacle(850, 980, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(880, 950, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(910, 920, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(940, 890, 80);
	obstacles.push_back(obs);

	obs = new Obstacle(970, 860, 80);
	obstacles.push_back(obs);
}