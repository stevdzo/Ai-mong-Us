#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define GRID_RATIO 50

#define ROWS (SCREEN_WIDTH / GRID_RATIO)
#define COLUMNS (SCREEN_HEIGHT / GRID_RATIO)

#define NODE_WIDTH GRID_RATIO
#define NODE_HEIGHT GRID_RATIO

#define SCALE_FACTOR GRID_RATIO

#define MAX_PLAYER_VELOCITY 150

#include "Vector2D.h"

#include <random>
#include <time.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <array>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "iostream"

#include <GL\glut.h>

struct Vector {

	int x, y;
};

struct Size {

	int width, height;
};

struct Color {

	float r, g, b;
};

inline void draw_text(char* string, float x, float y) {

	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(4.0f);
	char* c;
	glRasterPos3f(x, y, 0.0f);
	for (c = string; *c != '\0'; c++) {

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
}
#endif