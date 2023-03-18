#ifndef PATH_H
#define PATH_H

#include "GL\glut.h"
#include "Vector2D.h"
#include <vector>
#include <list>
#include <cassert>
#include "Agent.h"

class Path {

private:

	std::vector<Vector2D> path;
	std::vector<Vector2D>::iterator checkpoint;
	Agent* agent;

	bool is_looped;

public:

	Path() {

		/*for (size_t i = 0; i < 30; i++) {

			path.push_back(Vector2D(rand() % 800, rand() % 600));		
		}*/

		path.push_back(Vector2D(856, 520));
		path.push_back(Vector2D(881, 730));
		path.push_back(Vector2D(751, 883));
		path.push_back(Vector2D(542, 836));
		path.push_back(Vector2D(507, 707));
		path.push_back(Vector2D(640, 575));

		checkpoint = path.begin();
		is_looped = true;
	}

	Path(std::vector<Vector2D> _path) : path(_path) {

		checkpoint = path.begin();
		is_looped = false;
	}

	~Path() {

		delete agent;
	}

	Vector2D current_checkpoint() { return *checkpoint; }

	bool finished() { return !(checkpoint != path.end()); }

	bool last_checkpoint() {
		return *checkpoint == path.back();
	}

	void set_path(std::vector<Vector2D> p) { path.clear(); path = p; checkpoint = path.begin(); }
	std::vector<Vector2D> get_path() { return path; }

    inline void set_next_checkpoint() {

        if (++checkpoint == path.end()) {		
			if (is_looped) {
				checkpoint = path.begin();
			}
        }
    }

	void render_path() {
		
		glColor3f(1.0, 1.0, 0.0);

		std::vector<Vector2D>::const_iterator it = path.begin();
		Vector2D wp = *it++;

		while (it != path.end()) {

			glBegin(GL_LINES);
				glVertex2f(wp.x, wp.y);
				glVertex2f((*it).x, (*it).y);
			glEnd();

			wp = *it++;
		}
		if (is_looped) {
		
			glBegin(GL_LINES);
				glVertex2f((*(--it)).x, (*(--it)).y);
				glVertex2f((*path.begin()).x, (*path.begin()).y);			
			glEnd();
		}
		glColor3f(1.0, 1.0, 1.0);
	}
};
#endif