#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "Enums.h"
#include "Globals.h"

class GraphNode {

private:

    Vector2D position;
    Size size;
    int index = 0;
    int fcost;
    int gcost;  

    bool obstacle;

    double r, g, b;

public:

    GraphNode* parent;

    GraphNode(int index) : index(index) {}

    GraphNode(int index, Vector2D position) : index(index), position(position), obstacle(false) {

        size.height = 50;
        size.width = 50;
        fcost = 0;
        gcost = 0;
        //index++;      

        r = 0.0;
        g = 0.4;
        b = 1.0;
    }

    GraphNode(Vector2D position) : position(position), obstacle(false) {

        size.height = 50;
        size.width = 50;
        index++;
        parent = nullptr;
    }

    GraphNode() : index(invalid_index) {}

    int get_index() const { return index; }
    void set_index(int i) { index = i; }

    Vector2D get_position() { return position; };
    Size get_size() { return size; };

    int get_fcost() const { return fcost; }
    void set_fcost(int _cost) { fcost = _cost; }

    int get_gcost() const { return gcost; }
    void set_gcost(int _cost) { gcost = _cost; }

    void set_color(double _r, double _g, double _b) {
    
        r = _r;
        g = _g;
        b = _b;
    }

    bool is_obstacle() { return obstacle; }
    void is_obstacle(bool flag) { obstacle = flag; }

    void set_start_node() {

        obstacle = false;
        r = 0.0;
        g = 0.8;
        b = 0.2;
    }

    void set_target_node() {
    
        obstacle = false;
        r = 0.8;
        g = 0.2;
        b = 0.0;
    }

    void make_obstacle() {
    
        obstacle = true;
        r = 0.0;
        g = 0.0;
        b = 0.0;
    }


    void render() const {

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        double scale_factor = 100;
        glBegin(GL_QUADS);
        glColor4f(r, g, b, 0.2);
            glVertex2i(position.x * SCALE_FACTOR, position.y * SCALE_FACTOR);
            glVertex2i(position.x * SCALE_FACTOR + NODE_WIDTH, position.y * SCALE_FACTOR);
            glVertex2i(position.x * SCALE_FACTOR + NODE_WIDTH, position.y * SCALE_FACTOR + NODE_HEIGHT);
            glVertex2i(position.x * SCALE_FACTOR, position.y * SCALE_FACTOR + NODE_HEIGHT);
        glEnd();
      
        glLineWidth(GRID_RATIO / 50);
        glBegin(GL_LINE_LOOP);
        glColor4f(1, 1, 1, 0.2);
            glVertex2i(position.x * SCALE_FACTOR, position.y * SCALE_FACTOR);
            glVertex2i(position.x * SCALE_FACTOR + NODE_WIDTH, position.y * SCALE_FACTOR);
            glVertex2i(position.x * SCALE_FACTOR + NODE_WIDTH, position.y * SCALE_FACTOR + NODE_HEIGHT);
            glVertex2i(position.x * SCALE_FACTOR, position.y * SCALE_FACTOR + NODE_HEIGHT);
        glEnd();

        glColor3f(1, 1, 1);

        glDisable(GL_BLEND);

        /*char buffer[3];
        sprintf(buffer, "%i", index);
        draw_text(buffer, position.x * SCALE_FACTOR + 20, position.y * SCALE_FACTOR + 20);*/
    }  
};
#endif