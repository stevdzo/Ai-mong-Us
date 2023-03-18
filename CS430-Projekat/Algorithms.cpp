#include "Algorithms.h"
#include "Path.h"

AStar::AStar(Graph* _graph, GraphNode* _start_node, GraphNode* _target_node) :
											graph(_graph),
											start_node(_start_node),
											target_node(_target_node)
{
	start_node->set_color(0.0, 0.8, 0.2);
	target_node->set_color(0.8, 0.2, 0.0);
}

void AStar::retrace_path(GraphNode* _start, GraphNode* _end) {

	std::vector<GraphNode*> _path;

	GraphNode* current_node = _end;

	while (current_node != _start) {

		_path.push_back(current_node);
		current_node = current_node->parent;
	}
	std::reverse(_path.begin(), _path.end());

	path = _path;
}

Path* AStar::find_shortest_path() {

	if (!path.empty()) { path.clear(); }

	std::vector<GraphNode*> open_list;
	std::vector<GraphNode*> closed_list;

	open_list.push_back(start_node);

	start_node->set_fcost(heuristic_distance(start_node, target_node));

	while (!open_list.empty()) {

		GraphNode* current_node = lowest_cost_node(open_list, target_node);

		path.push_back(current_node);

		open_list.erase(std::find(open_list.begin(), open_list.end(), current_node));

		closed_list.push_back(current_node);

		if (current_node == target_node) {

			//retrace_path(start_node, target_node); 
			return new Path(nodes_position_path());
		}

		for (auto& child : get_neighbours(graph, current_node)) {

			if (std::find(closed_list.begin(), closed_list.end(), child) != closed_list.end()) { continue; }

			if (!child->is_obstacle() && std::find(open_list.begin(), open_list.end(), child) == open_list.end()) { open_list.push_back(child); }

			int possible_cost = current_node->get_gcost() + heuristic_distance(current_node, child);

			if (possible_cost < child->get_gcost()) {

				child->parent = current_node;
				child->set_gcost(possible_cost);
				child->set_fcost(child->get_gcost() + heuristic_distance(child, target_node));
			}
		}
	}
}

GraphNode* AStar::lowest_cost_node(std::vector<GraphNode*> _nodes, GraphNode* _target_node) { // trazim cvor sa najmanjom tezinom/cenom (f cost) f = g + h

	std::vector<int> costs;

	for (auto& i : _nodes) { costs.push_back(heuristic_distance(i, _target_node) + i->get_gcost()); }

	int lowest_cost = *std::min_element(costs.begin(), costs.end());

	for (auto& i : _nodes) {
	
		if (heuristic_distance(i, _target_node) + i->get_gcost() == lowest_cost) { return i; }
	}
	return nullptr;
}

std::vector<Vector2D> AStar::nodes_position_path() {

	std::vector<Vector2D> temp;
	for (auto& node : path) {

		temp.push_back(Vector2D((node->get_position().x * 100) / 2 + NODE_WIDTH / 2, 
								(node->get_position().y * 100) / 2 + NODE_HEIGHT / 2));
	}
	return temp;
}