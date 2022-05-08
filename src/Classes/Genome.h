#include "NeuralNet/Types.h"
#include "../Tools/Random.h"
#include "Utils.h"

namespace NeuralNet {
	extern Gene RandomGene();

	extern Genome RandomGenome();

	extern void MutateGene(Genome& genome);

	extern void MutateGenome(Genome& genome);

	extern Genome GenerateChild(std::vector<Genome> parents);
}