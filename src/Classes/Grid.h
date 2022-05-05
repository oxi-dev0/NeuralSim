#pragma once

#include <vector>
#include "Cell.h"
#include "Vector2D.h"

const uint16_t EMPTY = 0; // Index value 0 is reserved
const uint16_t BARRIER = 0xffff;

class Grid
{
public:
	std::vector<Cell> cells;

public:
	struct Column {
		Column(uint16_t numRows) : data{ std::vector<uint16_t>(numRows, 0) } {}
		void Clear() { std::fill(data.begin(), data.end(), 0); }
		uint16_t& operator[](uint16_t index) { return data[index]; }
		uint16_t operator[](uint16_t index) const { return data[index]; }
		size_t Size() const { return data.size(); }
	public:
		std::vector<uint16_t> data;
	};

	void Init(uint16_t sizeX, uint16_t sizeY);
	void Clear() { for (Column& col : columns) { col.Clear(); }; }
	uint16_t sizeX() const { return columns.size(); }
	uint16_t sizeY() const { return columns[0].Size(); }

	uint16_t at(Vector2D loc) const { return columns[loc.x][loc.y]; }
	uint16_t at(uint16_t x, uint16_t y) const { return columns[x][y]; }
	
	bool inBounds(Vector2D loc) const { return loc.x >= 0 && loc.x < sizeX() && loc.y >= 0 && loc.y < sizeY(); }
	bool isEmptyAt(Vector2D loc) const { return at(loc) == EMPTY; }
	bool isBarrierAt(Vector2D loc) const { return at(loc) == BARRIER; }

	bool isOccupiedAt(Vector2D loc) const { return at(loc) != EMPTY && at(loc) != BARRIER; }
	bool isBorder(Vector2D loc) const { return loc.x == 0 || loc.x == sizeX() - 1 || loc.y == 0 || loc.y == sizeY() - 1; }

	void set(Vector2D loc, uint16_t val) { columns[loc.x][loc.y] = val; }
	void set(uint16_t x, uint16_t y, uint16_t val) { columns[x][y] = val; }

	Vector2D findEmptyLocation() const;
	void createBarrier(unsigned barrierType);
	const std::vector<Vector2D>& getBarrierLocations() const { return barrierLocations; }
	const std::vector<Vector2D>& getBarrierCenters() const { return barrierCenters; }

	Column& operator[](uint16_t columnXNum) { return columns[columnXNum]; }
	const Column& operator[](uint16_t columnXNum) const { return columns[columnXNum]; }

private:
	std::vector<Column> columns;
	std::vector<Vector2D> barrierLocations;
	std::vector<Vector2D> barrierCenters;
};

