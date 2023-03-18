#ifndef GRAPHEDGDE_H
#define GRAPHEDGE_H

#include "Enums.h"
#include "Globals.h"
#include "GraphNode.h"
#include "GL/glut.h"

class GraphEdge {

protected:

    GraphNode* from;
    GraphNode* to;

    GraphNode* path_from;
    GraphNode* path_to;

	double cost;

public:

    GraphEdge() {}

    GraphEdge(GraphNode* from, GraphNode* to, double cost) : cost(cost),
                                                            from(from),
                                                            to(to)
    {}
    
    GraphEdge(GraphNode* from, GraphNode* to) : cost(1.0),

        from(from),
        to(to)
    {}

    GraphEdge(int from, int to, double cost) {
    
    }

    /*GraphEdge() : cost(1.0),
                  from(invalid_index),
                  to(invalid_index)
    {}*/

    int get_from()const { return from->get_index(); }
    GraphNode* get_from_node() { return from; }

    int get_to()const { return to->get_index(); }
    GraphNode* get_to_node() { return to; }

    void set_from_node(GraphNode* n) { from = n; }
    void set_to_node(GraphNode* n) { to = n; }

    GraphNode* get_path_to_node() { return path_to; }
    GraphNode* get_path_from_node() { return path_from; }

    double get_cost() const { return cost; }
    void set_cost(double _cost) { cost = _cost; }

    void render() const {

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glLineWidth(GRID_RATIO / 10.0f);
        glBegin(GL_LINES);
            glColor4f(0.7, 0.7, 0.7, 0.2);
            glVertex2i(from->get_position().x * SCALE_FACTOR + NODE_WIDTH/2, from->get_position().y * SCALE_FACTOR + NODE_HEIGHT/2);
            glVertex2i(to->get_position().x * SCALE_FACTOR + NODE_WIDTH/2, to->get_position().y * SCALE_FACTOR + NODE_HEIGHT/2);
        glEnd();   

        glDisable(GL_BLEND);
    }

    void render_path() {

        /*glPointSize(GRID_RATIO / 5.0f);
        glBegin(GL_POINT);
            glColor3f(0.1, 0.1, 0.1);
            glVertex2i(from->get_position().x * SCALE_FACTOR + NODE_WIDTH / 2, from->get_position().y * SCALE_FACTOR + NODE_HEIGHT / 2);
            glVertex2i(to->get_position().x * SCALE_FACTOR + NODE_WIDTH / 2, to->get_position().y * SCALE_FACTOR + NODE_HEIGHT / 2);
        glEnd();*/

        glLineWidth(GRID_RATIO / 5.0f);
        glBegin(GL_LINES);       
            glColor3f(0.0, 0.0, 0.0);
            glVertex2i(from->get_position().x * SCALE_FACTOR + NODE_WIDTH / 2, from->get_position().y * SCALE_FACTOR + NODE_HEIGHT / 2);
            glVertex2i(to->get_position().x * SCALE_FACTOR + NODE_WIDTH / 2, to->get_position().y * SCALE_FACTOR + NODE_HEIGHT / 2);
        glEnd();
        glLineWidth(GRID_RATIO / 10.0f);

        glColor3f(1.0, 1.0, 1.0);
    }

    bool operator==(const GraphEdge& rhs) const {

        return rhs.from == this->from &&
            rhs.to == this->to &&
            rhs.cost == this->cost;
    }

    bool operator!=(const GraphEdge& rhs) const {

        return !(*this == rhs);
    }
};
#endif