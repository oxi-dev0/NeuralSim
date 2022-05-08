#include "Cell.h"

void Cell::InitialiseGenome(NeuralNet::Genome genome) {
	map.FormMapFromGenome(genome);
}

void Cell::Step() {
	ProcessReceptors(); // Cache all needed input vals
	map.Execute(); // Loop through each connected effector, tracing back down the tree (using cached values to prevent recalculation where possible)
}