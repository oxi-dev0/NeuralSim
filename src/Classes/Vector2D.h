#pragma once

#include <stdint.h>
#include "Utils.h"

enum Compass {
	SW = 0,
	S,
	SE,
	W,
	CENTER,
	E,
	NW,
	N,
	NE
};

class Vector2D
{
public:
	int16_t x;
	int16_t y;

	Vector2D(int16_t x_, int16_t y_) { x = x_; y = y_; }
	Vector2D() { x = 0; y = 0; }

	Vector2D operator+(Vector2D const& v2) { return Vector2D(x + v2.x, y + v2.y); }
	Vector2D operator-(Vector2D const& v2) { return Vector2D(x - v2.x, y - v2.y); }
	Vector2D operator/(Vector2D const& v2) { return Vector2D(x / v2.x, y / v2.y); }
	Vector2D operator*(Vector2D const& v2) { return Vector2D(x * v2.x, y * v2.y); }

	Vector2D operator*(int const& i) { return Vector2D(x * i, y * i); }
	Vector2D operator*(float const& f) { return Vector2D((int16_t)(x * f), (int16_t)(y * f)); }
};

struct Dir {
public:
	Compass dir;

	Dir() { dir = Compass::SW; }
	Dir(Compass dir_) { dir = dir_; }
	Vector2D asNormalisedVect();
	int asInt() const;
	Dir rotate(int n) const;
};

const Vector2D NormalizedVects[9] = {
	Vector2D(-1,-1), // SW
	Vector2D(0,-1),  // S
	Vector2D(1,-1),  // SE
	Vector2D(-1,0),  // W
	Vector2D(0,0),   // CENTER
	Vector2D(1,0),   // E
	Vector2D(-1,1),  // NW
	Vector2D(0,1),   // N
	Vector2D(1,1)    // NE
}; 

const Dir rotations[72] = { SW, W, NW, N, NE, E, SE, S,
							S, SW, W, NW, N, NE, E, SE,
							SE, S, SW, W, NW, N, NE, E,
							W, NW, N, NE, E, SE, S, SW,
							CENTER, CENTER, CENTER, CENTER, CENTER, CENTER, CENTER, CENTER,
							E, SE, S, SW, W, NW, N, NE,
							NW, N, NE, E, SE, S, SW, W,
							N, NE, E, SE, S, SW, W, NW,
							NE, E, SE, S, SW, W, NW, N };

Dir toDir(Vector2D vec);
Vector2D RandomDir();