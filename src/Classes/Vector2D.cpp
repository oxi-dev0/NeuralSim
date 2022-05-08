#include "Vector2D.h"

Dir toDir(Vector2D vec) {
	constexpr uint16_t tanN = 13860;
	constexpr uint16_t tanD = 33461;
	const Dir conversion[16]{ S, CENTER, SW, N, SE, E, N,
								N, N, N, W, NW, N, NE, N, N };

	const int32_t xp = vec.x * tanD + vec.y * tanN;
	const int32_t yp = vec.y * tanD - vec.x * tanN;

	// We can easily check which side of the four boundary lines
	// the point now falls on, giving 16 cases, though only 9 are
	// possible.
	return conversion[(yp > 0) * 8 + (xp > 0) * 4 + (yp > xp) * 2 + (yp >= -xp)];
}

int Dir::asInt() const {
	return dir;
}

Vector2D Dir::asNormalisedVect() {
	return NormalizedVects[asInt()];
}

Dir Dir::rotate(int n) const
{
	return rotations[asInt() * 8 + (n & 7)];
}

Vector2D RandomDir() {
	Vector2D dir = Vector2D(0, 0);
	int dirV = Utils::RandRange(0, 7);
	switch (dirV) {
	case 0:
		dir = Vector2D(0, 1);
		break;
	case 1:
		dir = Vector2D(1, 1);
		break;
	case 2:
		dir = Vector2D(1, 0);
		break;
	case 3:
		dir = Vector2D(1, -1);
		break;
	case 4:
		dir = Vector2D(0, -1);
		break;
	case 5:
		dir = Vector2D(-1, -1);
		break;
	case 6:
		dir = Vector2D(-1, 0);
		break;
	case 7:
		dir = Vector2D(-1, 1);
		break;
	}
	return dir;
}
