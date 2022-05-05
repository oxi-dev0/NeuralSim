#pragma once

class Vector2D
{
public:
	int16_t x;
	int16_t y;

	Vector2D(int16_t x_, int16_t y_) { x = x_; y = y_; }
	Vector2D() { x = 0; y = 0; }

	Vector2D operator+(Vector2D const& v1, Vector2D const& v2) { return Vector2D(v1.x + v2.x, v1.y + v2.y); }
	Vector2D operator-(Vector2D const& v1, Vector2D const& v2) { return Vector2D(v1.x - v2.x, v1.y - v2.y); }
	Vector2D operator/(Vector2D const& v1, Vector2D const& v2) { return Vector2D(v1.x / v2.x, v1.y / v2.y); }
	Vector2D operator*(Vector2D const& v1, Vector2D const& v2) { return Vector2D(v1.x * v2.x, v1.y * v2.y); }
};

