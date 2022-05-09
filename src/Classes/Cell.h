#pragma once

#include <cstdint>

#include "Vector2D.h"
#include "NeuralNet.h"

class Cell
{
public:
	Vector2D pos; // Pos in grid[][]
	Dir dir;
	int id;
	int oscPeriod = 34;
	bool dead;
	NeuralNet::Genome genome;

	std::shared_ptr<NeuralNet::NeuralMap> map;

	Cell(Vector2D pos_) { pos = pos_; dir = Dir(CENTER); id = -1; dead = false; }
	void InitaliseMap() { map = std::shared_ptr<NeuralNet::NeuralMap>(new NeuralNet::NeuralMap(id)); };

	void InitialiseGenome(NeuralNet::Genome genome);
	void Step();
	void ProcessReceptors();
	
	float GetReceptorVal(NeuralNet::Receptor receptor);
};