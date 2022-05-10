#pragma once

#include <cstdint>
#include "../Utils.h"

namespace NeuralNet {

	enum NeuronType {
		RECEPTOR,
		INTERNAL,
		EFFECTOR
	};

	enum Receptor {
		LOC_X,
		LOC_Y,
		RANDOM,
		AGE,
		OSC,
		RECEPTORCOUNT // Used to determine the total amount of receptors. Add new receptors before this
	};

	enum Effector {
		MOVE_X,
		MOVE_Y,
		MOVE_FB, // parralel last moved dir
		MOVE_LR, // perp from last moved dir
		MOVE_RANDOM,
		MOVE_NORTH,
		MOVE_EAST,
		MOVE_SOUTH,
		MOVE_WEST,
		ACTIONCOUNT // Used to determine the total amount of effector. Add new effectors before this
	};

	class Gene {
	public:
		uint16_t sourceId = 0; // Index of source in neuron list
		uint16_t sinkId = 0; // Index of source in neuron list
		int16_t weight = 0; // Weight of gene from -4 to 4 float * 8192

		Gene() { sourceId = 0; sinkId = 0; weight = 0; }
		Gene(uint16_t soI, uint16_t siI, float w) { sourceId = soI; sinkId = siI; weight = (int)w * 8192; }

		float weightFloat() { return weight / 8192.0f; }
		static float randWeight() { return ((float)Utils::RandRange(0, 0xffff) - 0x8000) / 8192.0f; }
	};

	typedef std::vector<Gene> Genome;

	struct NeuronNode {
		NeuronType type;
		int id; // Index in neuron list
		
		std::vector<int> in; // List of neuron indices
		std::vector<float> inWeights;
		bool connected = false;
		bool connectedIn = false;

		Receptor receptorType;
		Effector effectorType;

		NeuronNode() { type = NeuronType::EFFECTOR; id = -1; receptorType = Receptor::RECEPTORCOUNT; effectorType = Effector::ACTIONCOUNT; }
		NeuronNode(NeuronType type_, int id_, Receptor receptorType_, Effector effectorType_) { type = type_; receptorType = receptorType_; id = id_; effectorType = effectorType_; }

		float eval(const std::vector<std::shared_ptr<NeuronNode>> Nodes, std::unordered_map<int, float>* vals, std::unordered_map<int, float>* prevVals, std::vector<int> visited);
	};
}