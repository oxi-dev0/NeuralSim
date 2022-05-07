#include "Grid.h"

void Grid::Init(uint16_t sizeX, uint16_t sizeY)
{
	Column col = Column(sizeY);
	columns = std::vector<Column>(sizeX, col);
}

Vector2D Grid::findEmptyLocation() const {
    Vector2D loc;

    while (true) {
        loc.x = Utils::RandRange(0, Utils::GlobalConfig.sizeX - 1);
        loc.y = Utils::RandRange(0, Utils::GlobalConfig.sizeY - 1);
        if (isEmptyAt(loc)) {
            break;
        }
    }
    return loc;
}