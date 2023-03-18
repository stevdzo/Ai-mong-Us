#ifndef Vector2D_H
#define Vector2D_H

#include <ctime>
#include <math.h>
#include "Utils.h"

class Vector2D {

public:

	double x, y;

	Vector2D() {

		x = 0; y = 0;
	}

	Vector2D(double _x, double _y) {

		x = _x;
		y = _y;
	}

	void add(Vector2D _v) {

		x += _v.x;
		y += _v.y;
	}

	void add(Vector2D _v1, Vector2D _v2) {

		x = _v1.x + _v2.x;
		y = _v1.y + _v2.y;
	}

	void subtract(Vector2D _v) {

		x -= _v.x;
		y -= _v.y;
	}

	void multiply(double _scalar) {

		x *= _scalar;
		y *= _scalar;
	}

	void divide(double _scalar) {

		x /= _scalar;
		y /= _scalar;
	}

	inline double dot(const Vector2D& v2)const
	{
		return x * v2.x + y * v2.y;
	}

	inline double magnitude() const {

		return sqrt(x * x + y * y);
	}

	inline double magnitude_sq() const {

		return (x * x + y * y);
	}

	inline double distance_to(const Vector2D& v2) const {
		double ySep = v2.y - y;
		double xSep = v2.x - x;

		return sqrt((xSep * xSep) + (ySep * ySep));
	}

	inline double distance_to_sq(const Vector2D& v2) const {

		double ySep = v2.y - y;
		double xSep = v2.x - x;

		return xSep * xSep + ySep * ySep;
	}

	Vector2D vec_normalize() const {
	
		Vector2D v = *this;
		v /= this->magnitude();
		return v;
	}

	inline void normalize() {			

		double vector_length = this->magnitude();

		if (vector_length > std::numeric_limits<double>::epsilon()) {

			this->x /= vector_length;
			this->y /= vector_length;
		}
	}

	inline double multiply(const Vector2D& v2)const
	{
		return (x * v2.x) + (y * v2.y);
	}	

	inline double Vec2DDistanceSq(const Vector2D& v1, const Vector2D& v2)
	{

		double ySeparation = v2.y - v1.y;
		double xSeparation = v2.x - v1.x;

		return ySeparation * ySeparation + xSeparation * xSeparation;
	}

	inline void truncate(double max) {

		/*if (magnitude() > max) {
			Vector2D norm = this->normalize();
			x = norm.x;
			y = norm.y;
			*this *= max;
		}*/
		if (this->magnitude() > max)
		{
			this->normalize();

			*this *= max;
		}
	}

	void zero() {
		x = 0;
		y = 0;
	}

	bool is_zero()const { return (x * x + y * y) < min_double; }

	enum { clockwise = 1, anticlockwise = -1 };

	inline int sign(const Vector2D& v2)const
	{
		if (y * v2.x > x* v2.y)
		{
			return anticlockwise;
		}
		else
		{
			return clockwise;
		}
	}

	inline Vector2D perp()const
	{
		return Vector2D(-y, x);
	}

	void wrap_around(int MaxX, int MaxY) {

		if (x > MaxX) { x = 0.0; }

		if (x < 0) { x = (double)MaxX; }

		if (y < 0) { y = (double)MaxY; }

		if (y > MaxY) { y = 0.0; }
	}

	const Vector2D operator+=(const Vector2D& v2) {

		x += v2.x;
		y += v2.y;

		return *this;
	}

	const Vector2D operator-=(const Vector2D& v2) {

		x -= v2.x;
		y -= v2.y;

		return *this;
	}

	const Vector2D operator-(const Vector2D& v2) const {

		double new_x = x - v2.x;
		double new_y = y - v2.y;

		return Vector2D(new_x, new_y);
	}

	const Vector2D operator+(const Vector2D& v2) const {
		double new_x = x + v2.x;
		double new_y = y + v2.y;

		return Vector2D(new_x, new_y);
	}

	const Vector2D operator*=(const double scalar) {
		x *= scalar;
		y *= scalar;

		return *this;
	}

	bool operator==(const Vector2D& v2) const {
		return (x == v2.x) && (y == v2.y);
	}

	/*const Vector2D operator*(const double scalar) const {

		double new_x = x * scalar;
		double new_y = y * scalar;

		return Vector2D(new_x, new_y);
	}	*/

	const Vector2D operator/=(const double scalar) {
		x /= scalar;
		y /= scalar;

		return *this;
	}

	const Vector2D operator/(const double scalar) const {

		double new_x = x / scalar;
		double new_y = y / scalar;

		return Vector2D(new_x, new_y);
	}

	void operator= (const Vector2D& _v) {
		x = _v.x;
		y = _v.y;
	}
};

inline Vector2D normalize_2d(const Vector2D& v) {

	Vector2D vec = v;

	double vector_length = vec.magnitude();

	if (vector_length > std::numeric_limits<double>::epsilon()) {

		vec.x /= vector_length;
		vec.y /= vector_length;
	}
	return vec;
}

inline double vec_distance(const Vector2D& v1, const Vector2D& v2) {

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
}

inline double vec_distance_sq(const Vector2D& v1, const Vector2D& v2) {

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return ySeparation * ySeparation + xSeparation * xSeparation;
}

inline void wrap_around(Vector2D& pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX) { pos.x = 0.0; }

	if (pos.x < 0) { pos.x = (double)MaxX; }

	if (pos.y < 0) { pos.y = (double)MaxY; }

	if (pos.y > MaxY) { pos.y = 0.0; }
}

inline void boundaries(Vector2D& pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX - 30) { pos.x = MaxX - 30; }

	if (pos.x < 30) { pos.x = 30; }

	if (pos.y < 0) { pos.y = 0; }

	if (pos.y > MaxY) { pos.y = MaxY; }
}

inline void boundaries(Vector2D& pos, Vector2D& vel, double radius, int MaxX, int MaxY) {

	if (pos.x + radius > MaxX - 30) { vel.x = -vel.x; pos.x = MaxX - 30 - radius; }

	if (pos.x - radius < 30) { vel.x = -vel.x; pos.x = 30 + radius; }

	if (pos.y - radius < 30) { vel.y = -vel.y; pos.y = 30 + radius; }

	if (pos.y + radius > MaxY - 60) { vel.y = -vel.y; pos.y = MaxY - 60 - radius; }
}

inline void player_boundaries(Vector2D& pos, Vector2D& vel, double radius, int MaxX, int MaxY) {

	if (pos.x + radius > MaxX - 30) { pos.x = MaxX - 30 - radius; }

	if (pos.x - radius < 30) { pos.x = 30 + radius; }

	if (pos.y - radius < 30) { pos.y = 30 + radius; }

	if (pos.y + radius > MaxY - 60) { pos.y = MaxY - 60 - radius; }
}

inline Vector2D operator*(const Vector2D& lhs, double rhs) {

	Vector2D result(lhs);
	result *= rhs;
	return result;
}

inline Vector2D operator*(double lhs, const Vector2D& rhs) {

	Vector2D result(rhs);
	result *= lhs;
	return result;
}


#endif