#include "Graph.h"

void Graph::add_node(GraphNode* node) {

    this->nodes.push_back(node);
    this->edges.push_back(std::list<GraphEdge>());
}

void Graph::add_edge(const GraphEdge& edge) {

    std::list<GraphEdge> from_list = this->edges[edge.get_from()];
    std::list<GraphEdge> to_list = this->edges[edge.get_to()];

    if (edge.get_from() == edge.get_to()) { return; }

    for (std::list<GraphEdge>::const_iterator i = from_list.begin(); i != from_list.end(); ++i) {

        if ((*i) == edge) { return; }
    }

    for (std::list<GraphEdge>::const_iterator i = to_list.begin(); i != to_list.end(); ++i) {

        if ((*i) == edge) { return; }
    }
    this->edges[edge.get_to()].push_back(edge);
}

void Graph::render() const {

    for (unsigned int i = 0; i < this->nodes.size(); i++) {

        this->nodes.at(i)->render();
    }

    for (unsigned int i = 0; i < this->edges.size(); i++) {

        std::list<GraphEdge> list = this->edges.at(i);
        std::list<GraphEdge>::const_iterator iterator;

        for (iterator = list.begin(); iterator != list.end(); ++iterator) {

            (*iterator).render();  
        }
    }
}

void Graph::display_path(std::vector<GraphNode*> nodes) {

    for (size_t i = 1; i < nodes.size(); i++) {

        GraphEdge temp;
        temp.set_from_node(nodes[i - 1]);
        temp.set_to_node(nodes[i]);

        temp.render_path();
    }
}

void Graph::set_nodes(const std::vector<GraphNode*>& nodes) {

    this->nodes.empty();
    this->edges.empty();
    this->nodes = nodes;

    for (int i = 0; i < nodes.size(); i++) {

        this->edges.push_back(std::list<GraphEdge>());
    }
}

GraphEdge Graph::get_edge(const GraphNode* from, const GraphNode* to) const {

    std::list<GraphEdge> con;
    auto invalid_edge = GraphEdge(0, 0);

    try {
        con = this->edges.at(static_cast<unsigned long>(from->get_index()));
    }
    catch (const std::out_of_range&) {

        return invalid_edge;
    }

    for (auto it = con.begin(); it != con.end(); ++it) {

        if ((*it).get_to() == to->get_index()) { return *it; }
    }
    return invalid_edge;
} 

std::vector<std::vector<GraphNode*>> Graph::get_nodes_as_matrix() const { // vracam 2D vektor iz 1D vektora kako bih mogao da odredim komsijske cvorove
    
    std::vector<std::vector<GraphNode*>> matrix(ROWS, std::vector<GraphNode*>(COLUMNS));

    int index = -1;
    for (size_t rows = 0; rows < ROWS; rows++) {

        for (size_t columns = 0; columns < COLUMNS; columns++) {

            matrix[rows][columns] = nodes[++index];
        }
    }
    return matrix;
}