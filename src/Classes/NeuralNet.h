#pragma once

#include <vector>

#include "Utils.h"
#include "Queues.h"
#include "../Tools/Log.h"

#include "NeuralNet/Types.h"

namespace NeuralNet {
	class NeuralMap {
	public:
		std::vector<std::shared_ptr<NeuronNode>> Nodes;
		std::unordered_map<int, float>* vals = new std::unordered_map<int, float>();
		std::unordered_map<int, float>* prevVals = new std::unordered_map<int, float>();

		bool initialised = false;

		int owner;

		NeuralMap(int owner_) { owner = owner_; if (!initialised) { InitialiseNodesList(); } }
		NeuralMap() {}// if (!initialised) { InitialiseNodesList(); } }
		~NeuralMap() { vals->clear(); prevVals->clear(); delete vals; delete prevVals; }

		void InitialiseNodesList();

		void FormMapFromGenome(std::vector<Gene> Genome);

		void Execute();
	};
}