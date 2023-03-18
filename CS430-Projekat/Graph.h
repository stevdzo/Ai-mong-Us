#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <cassert>
#include <string>
#include <iostream>

#include "GraphEdge.h"
#include "GraphNode.h"

class Graph {

private:

    std::vector<GraphNode*> nodes;
    std::vector<std::list<GraphEdge> > edges;

public:

    Graph() : nodes(std::vector<GraphNode*>()),
              edges(std::vector<std::list<GraphEdge> >())
    {}

    void add_node(GraphNode*);
    void add_edge(const GraphEdge&);

    void render() const;

    std::vector<GraphNode*> get_nodes() const { return nodes; };

    std::vector<std::vector<GraphNode*>> get_nodes_as_matrix() const;

    std::vector<std::list<GraphEdge> > get_edges() const { return edges; }

    void set_nodes(const std::vector<GraphNode*>& nodes);

    void display_path(std::vector<GraphNode*>);

    GraphEdge get_edge(const GraphNode* from, const GraphNode* to) const;

    GraphNode* node_clicked(int x, int y) {

        for (size_t i = 0; i < nodes.size(); i++) {
            
            if ((x < nodes[i]->get_position().x * SCALE_FACTOR + NODE_WIDTH &&
                 x > nodes[i]->get_position().x * SCALE_FACTOR) &&
                (y < nodes[i]->get_position().y * SCALE_FACTOR + NODE_HEIGHT &&
                 y > nodes[i]->get_position().y * SCALE_FACTOR)) {

                return nodes[i];
            }
        }      
        return nullptr;
    }
};
#endif