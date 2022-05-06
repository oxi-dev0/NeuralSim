#include "Grid.h"

void Grid::Init(uint16_t sizeX, uint16_t sizeY)
{
	Column col = Column(sizeY);
	columns = std::vector<Column>(sizeX, col);
}

Vector2D Grid::findEmptyLocation() const {
    Vector2D loc;

    while (true) {
        loc.x = Globals::RandRange(0, Globals::GlobalConfig.sizeX - 1);
        loc.y = Globals::RandRange(0, Globals::GlobalConfig.sizeY - 1);
        if (isEmptyAt(loc)) {
            break;
        }
    }
    return loc;
}