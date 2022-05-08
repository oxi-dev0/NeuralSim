#pragma once

#include <vector>

#include "Utils.h"
#include "Queues.h"
#include "../Tools/Log.h"

#include "NeuralNet/Types.h"

namespace NeuralNet {
	class NeuralMap {
	public:
		std::vector<NeuronNode*> Nodes;
		std::unordered_map<int, float> vals;
		std::unordered_map<int, float> prevVals;

		int owner;

		NeuralMap(int owner_) { owner = owner_; InitialiseNodesList(); }
		NeuralMap() { InitialiseNodesList(); }

		void InitialiseNodesList();

		void FormMapFromGenome(std::vector<Gene> Genome);

		void Execute();
	};
}