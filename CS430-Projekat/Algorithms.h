#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include "Globals.h"

class Path;

class AStar {

	Graph* graph;
	int f_cost, g_cost, heuristics;
	std::vector<GraphNode*> path;
	GraphNode* start_node;
	GraphNode* target_node;

public:
	
	AStar() {}

	AStar(Graph*, GraphNode*, GraphNode*);

	void retrace_path(GraphNode*, GraphNode*);
	Path* find_shortest_path();
	GraphNode* lowest_cost_node(std::vector<GraphNode*>, GraphNode*);

	void set_graph(Graph* g) { graph = g; }
	std::vector<GraphNode*> get_path() { return path; };

	void set_start_node(GraphNode* n) { start_node = n; /*start_node->set_start_node();*/ }
	void set_target_node(GraphNode* n) { target_node = n; /*target_node->set_target_node();*/ }

	GraphNode* get_start_node() { return start_node; }
	GraphNode* get_target_node() { return target_node; }

	std::vector<Vector2D> nodes_position_path();
};

inline std::vector<GraphNode*> get_neighbours(Graph* graph, GraphNode* _node) { // odredjujem komsijske cvorove datog cvora 
																				//i odredjujem G_COST za iste zavisno od toga gde se nalaze u odnosu na dati cvor 
																				//14 dijagonalno, 10 iznad, ispod i levo, desno (pitagora)

	std::vector<GraphNode*> neighbours;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0) continue;

			int check_x = _node->get_position().x + x;
			int check_y = _node->get_position().y + y;

			if (check_x > -1 && check_x < ROWS && check_y > -1 && check_y < COLUMNS) {

				if (x == 0 || y == 0) { graph->get_nodes_as_matrix()[check_x][check_y]->set_gcost(10); }
				else { graph->get_nodes_as_matrix()[check_x][check_y]->set_gcost(14); }

				neighbours.push_back(graph->get_nodes_as_matrix()[check_x][check_y]);
			}
		}
	}
	return neighbours;
}

inline int heuristic_distance(GraphNode* _a, GraphNode* _b) { // racunam heuristiku izmedju dva cvora na sledeci nacin

	int dist_x = std::abs(_a->get_position().x - _b->get_position().x);
	int dist_y = std::abs(_a->get_position().y - _b->get_position().y);

	if (dist_x > dist_y) {

		return 14 * dist_y + 10 * (dist_x - dist_y);
	}
	else
		return 14 * dist_x + 10 * (dist_y - dist_x);
}
#endif