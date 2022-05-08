#pragma once

#include <cstdint>

#include "Vector2D.h"
#include "NeuralNet.h"

class Cell
{
public:
	Vector2D pos; // Pos in grid[][]
	Vector2D dir;
	int id;

	NeuralNet::NeuralMap map;

	Cell(Vector2D pos_) { pos = pos_; dir = Vector2D(1, 0); id = -1; }
	void InitaliseMap() { map = NeuralNet::NeuralMap(id); };

	void InitialiseGenome(NeuralNet::Genome genome);
	void Step();
	void ProcessReceptors();
	
	float GetReceptorVal(NeuralNet::Receptor receptor);
};