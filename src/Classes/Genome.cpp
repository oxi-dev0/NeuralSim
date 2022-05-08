#include "Genome.h"

namespace NeuralNet {
	Gene RandomGene() {
		uint16_t sourceType = Utils::RandRange(0, 1); // 0 = Receptor; 1 = Internal
		uint16_t sinkType = Utils::RandRange(0, 1); // 0 = Effector; 1 = Internal

		Gene newGene = Gene();
		newGene.sourceId = sourceType == 0 ? Utils::RandRange(0, (Receptor::RECEPTORCOUNT - 1)) : Utils::RandRange(Receptor::RECEPTORCOUNT, (Receptor::RECEPTORCOUNT - 1) + Utils::GlobalConfig.internalNeuronCount);
		newGene.sinkId = sinkType == 0 ? Utils::RandRange((Receptor::RECEPTORCOUNT)+Utils::GlobalConfig.internalNeuronCount, (Receptor::RECEPTORCOUNT - 1) + Utils::GlobalConfig.internalNeuronCount + (Effector::ACTIONCOUNT - 1)) : Utils::RandRange(Receptor::RECEPTORCOUNT, (Receptor::RECEPTORCOUNT - 1) + Utils::GlobalConfig.internalNeuronCount);
		newGene.weight = Gene::randWeight() * 8192;
		return newGene;
	}

	Genome RandomGenome() {
		Genome newGenome;
		int length = Utils::GlobalConfig.genomeLength;
		for (int g = 0; g < length; g++) {
			newGenome.push_back(RandomGene());
		}
		return newGenome;
	}

	void MutateGene(Genome& genome) {
		unsigned geneIndex = Utils::RandRange(0, genome.size() - 1);
		uint8_t bitIndex8 = 1 << Utils::RandRange(0, 7);

		float chance = Utils::RandRange() / (float)Random::RANDOM_UINT_MAX;
		if (chance < 0.33) {
			uint8_t sourceType = genome[geneIndex].sourceId < Receptor::RECEPTORCOUNT ? 0 : 1;
			genome[geneIndex].sourceId ^= bitIndex8;
			genome[geneIndex].sourceId %= sourceType == 0 ? Receptor::RECEPTORCOUNT : Utils::GlobalConfig.internalNeuronCount + 1;
			genome[geneIndex].sourceId += sourceType == 1 ? Receptor::RECEPTORCOUNT : 0;
		}
		else {
			if (chance < 0.66) {
				uint8_t destType = genome[geneIndex].sinkId > (Receptor::RECEPTORCOUNT - 1) + Utils::GlobalConfig.internalNeuronCount ? 0 : 1;
				genome[geneIndex].sinkId ^= bitIndex8;
				genome[geneIndex].sinkId %= destType == 0 ? Effector::ACTIONCOUNT : Utils::GlobalConfig.internalNeuronCount + 1;
				genome[geneIndex].sinkId += destType == 1 ? Receptor::RECEPTORCOUNT : Receptor::RECEPTORCOUNT + Utils::GlobalConfig.internalNeuronCount;
			}
			else {
				genome[geneIndex].weight ^= (1 << Utils::RandRange(1, 15));
			}
		}
	}

	void MutateGenome(Genome& genome) {
		unsigned geneCount = genome.size();
		while (geneCount-- > 0) {
			if (Utils::RandRange() / (float)Random::RANDOM_UINT_MAX < Utils::GlobalConfig.mutationChance) {
				MutateGene(genome);
			}
		}
	}

	Genome GenerateChild(std::vector<Genome> parents) {
		Genome child;

		uint16_t parent1Idx;
		uint16_t parent2Idx;

		if (parents.size() == 0) {
			LOG_CRITICAL("ZERO CELLS SURVIVED");
		}

		parent1Idx = Utils::RandRange(0, parents.size() - 1);
		parent2Idx = Utils::RandRange(0, parents.size() - 1);

		Genome parent1 = parents[parent1Idx];
		Genome parent2 = parents[parent2Idx];

		for (int g = 0; g < parent1.size(); g++) {
			bool par = (bool)Utils::RandRange(0, 1);
			child.push_back(par ? parent1[g] : parent2[g]);
		}

		MutateGenome(child);

		return child;
	}
}