#ifndef MOVING_ENTITY_H
#define MOVING_ENTITY_H

#include <cassert>

//#include "Vector2D.h"
#include "BaseGameEntity.h"

class MovingEntity : public BaseGameEntity {

protected:

	Vector2D velocity;
	Vector2D heading;
    Vector2D side;

	double mass;
	double max_speed;
	double max_force;
	double max_turn_rate;

    int health;
    bool dead;

public:

    MovingEntity(Vector2D position,
                 double    radius,
                 Vector2D  velocity,
                 double    max_speed,
                 Vector2D  heading,
                 double    mass,
                 Vector2D  scale,
                 double    turn_rate,
                 double    max_force) : BaseGameEntity(0, position, radius),
                                       heading(heading),
                                       velocity(velocity),
                                       mass(mass),
                                       side(heading.perp()),
                                       max_speed(max_speed),
                                       max_turn_rate(turn_rate),
                                       max_force(max_force)
    {
        this->scale = scale;
        health = 100;
    }

    virtual ~MovingEntity() {}

    virtual bool add_walk_texture(const char*, const bool) = 0;
    virtual bool add_idle_texture(const char*, const bool) = 0;
    virtual bool add_death_texture(const char*, const bool) = 0;

    Vector2D  get_velocity()const { return velocity; }
    void set_velocity(const Vector2D& vel) { velocity = vel; }

    double get_mass()const { return mass; }

    Vector2D get_side()const { return side; }

    double get_max_speed() const { return max_speed; }
    void set_max_speed(double new_speed) { max_speed = new_speed; }

    double get_max_force() const { return max_force; }
    void set_max_force(double mf) { max_force = mf; }

    int get_health() const { return health; }
    void set_health(int _h) { health = _h; }

    bool is_dead() const { return dead; }
    void is_dead(bool _d) { dead = _d; }

    bool is_speed_maxed_out()const { return max_speed * max_speed >= velocity.magnitude_sq(); }
    double speed()const { return velocity.magnitude(); }
    double speed_sq()const { return velocity.magnitude_sq(); }

    Vector2D get_heading()const { return heading; }
    void set_heading(Vector2D new_heading);
    bool rotate_heading_to_face_position(Vector2D target);

    double get_max_turn_rate()const { return max_turn_rate; }
    void set_max_turn_rate(double val) { max_turn_rate = val; }
};

inline void MovingEntity::set_heading(Vector2D new_heading) {

    assert((new_heading.magnitude_sq() - 1.0) < 0.00001);

    heading = new_heading;
    side = heading.perp();
}

inline bool MovingEntity::rotate_heading_to_face_position(Vector2D target) {

    Vector2D to_target = normalize_2d(target - position);

    double angle = acos(heading.dot(to_target));

    if (angle < 0.00001) return true;

    if (angle > max_turn_rate) angle = max_turn_rate;

    Matrix2D rotation_matrix;

    rotation_matrix.rotate(angle * heading.sign(to_target));
    rotation_matrix.transform_vector_2d(heading);
    rotation_matrix.transform_vector_2d(velocity);

    side = heading.perp();

    return false;
}
#endif