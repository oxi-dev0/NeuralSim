#include "../Cell.h"

void Cell::ProcessReceptors() {
	for (NeuralNet::NeuronNode* Node : map.Nodes) {
		if (!Node->type == NeuralNet::NeuronType::RECEPTOR) {
			continue;
		}

		if (!Node->connected) {
			continue;
		}

		NeuralNet::Receptor receptor = Node->receptorType;
		float val = GetReceptorVal(receptor);
		map.vals[Node->id] = val;
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
	}
}