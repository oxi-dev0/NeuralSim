#include "Cell.h"

void Cell::InitialiseGenome(NeuralNet::Genome genome_) {
	genome = genome_;
	map->FormMapFromGenome(genome_);
}

void Cell::Step() {
	map->prevVals->clear();
	for (auto kv : *(map->vals)) {
		map->prevVals->operator[](kv.first) = kv.second;
	}
	map->vals->clear();
	ProcessReceptors(); // Cache all needed input vals
	map->Execute(); // Loop through each connected effector, tracing back down the tree (using cached values to prevent recalculation where possible)
}