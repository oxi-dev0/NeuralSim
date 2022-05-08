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

void Grid::MoveCell(Vector2D oldPos, Vector2D offset) {
    uint16_t id = at(oldPos);
    Cell* cell = cells[id];
    cell->pos = cell->pos + offset;
    if (cell->pos.x >= columns.size() || cell->pos.y >= columns[0].Size()) {
        set(cell->pos - offset, 0);
        delete cell;
    }
    else {
        set(cell->pos - offset, 0);
        set(cell->pos, id);
    }
}