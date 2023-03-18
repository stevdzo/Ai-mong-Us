#include "Obstacle.h"

Obstacle::Obstacle(double x, double y, double r) : BaseGameEntity(0, Vector2D(x, y), r) {}

Obstacle::Obstacle(Vector2D pos, double r) : BaseGameEntity(0, pos, r) {}

Obstacle::~Obstacle() {}

void Obstacle::update(double delta_time) {


}

void Obstacle::render() {
   
    double angle;

    glBegin(GL_LINE_LOOP);
    glColor3f(1, 1, 1);
    for (size_t i = 0; i < 360; i++) {

        angle = 2 * PI * i / 360;
        glVertex2f(position.x + bounding_radius * cos(angle), position.y + bounding_radius * sin(angle));
    }
    glEnd();

    //glColor3f(1, 1, 1);
}