#include "../Cell.h"

void Cell::ProcessReceptors() {
	// For every node
	for (auto Node : map->Nodes) {
		if (!Node->type == NeuralNet::NeuronType::RECEPTOR) {
			// If it is not a receptor, skip
			continue;
		}

		if (!Node->connected) {
			// If it is not connected to anything, skip
			continue;
		}

		// Get the receptor value and add it to the value cache
		NeuralNet::Receptor receptor = Node->receptorType;
		float val = GetReceptorVal(receptor);
		map->vals->operator[](Node->id) = val;
	}
}

float Cell::GetReceptorVal(NeuralNet::Receptor receptor) {
	switch (receptor) {
	case NeuralNet::Receptor::LOC_X:
		return (float)pos.x / Utils::GlobalConfig.sizeX;
		break;
	case NeuralNet::Receptor::LOC_Y:
		return (float)pos.y / Utils::GlobalConfig.sizeY;
		break;
	case NeuralNet::Receptor::AGE:
		return (float)Utils::GlobalSimData.currentStep / Utils::GlobalConfig.stepsPerGen;
		break;
	case NeuralNet::Receptor::RANDOM:
		return (float)Utils::RandRange() / Random::RANDOM_UINT_MAX;
		break;
	case NeuralNet::Receptor::OSC:
		float phase = (Utils::GlobalSimData.currentStep % oscPeriod) / (float)oscPeriod; // 0.0..1.0
		float factor = -std::cos(phase * 2.0f * 3.1415927f);
		assert(factor >= -1.0f && factor <= 1.0f);
		factor += 1.0f;    // convert to 0.0..2.0
		factor /= 2.0;     // convert to 0.0..1.0
		return std::min<float>(1.0, std::max<float>(0.0, factor));
	}
	return 0.0f;
}