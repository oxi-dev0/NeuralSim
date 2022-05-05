#include "Grid.h"

void Grid::Init(uint16_t sizeX, uint16_t sizeY)
{
	Column col = Column(sizeY);
	columns = std::vector<Column>(sizeX, col);
}

Vector2D Grid::findEmptyLocation() const {
    Vector2D loc;

    while (true) {
        loc.x = randomUint(0, p.sizeX - 1);
        loc.y = randomUint(0, p.sizeY - 1);
        if (isEmptyAt(loc)) {
            break;
        }
    }
    return loc;
}