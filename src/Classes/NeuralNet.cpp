#include "NeuralNet.h"

namespace NeuralNet {

	void NeuralMap::InitialiseNodesList() {
		// Generate the array of neurons that genes reference using the type enums

		initialised = true;
		Nodes.clear();
		vals->clear();

		for (int receptorI = Receptor::LOC_X; receptorI != Receptor::RECEPTORCOUNT; receptorI++)
		{
			NeuronNode* node = new NeuronNode(NeuronType::RECEPTOR, -1, static_cast<Receptor>(receptorI), Effector::ACTIONCOUNT);
			int id = Nodes.size();
			Nodes.push_back(std::shared_ptr<NeuronNode>(node));
			node->id = id;
			vals->operator[](id)=0.5;
		}

		for (int c = 0; c < Utils::GlobalConfig.internalNeuronCount; c++)
		{
			NeuronNode* node = new NeuronNode(NeuronType::INTERNAL, -1, Receptor::RECEPTORCOUNT, Effector::ACTIONCOUNT);
			int id = Nodes.size();
			Nodes.push_back(std::shared_ptr<NeuronNode>(node));
			node->id = id;
			vals->operator[](id) = 0.5;
		}

		for (int effectorI = Effector::MOVE_X; effectorI != Effector::ACTIONCOUNT; effectorI++)
		{
			NeuronNode* node = new NeuronNode(NeuronType::EFFECTOR, -1, Receptor::RECEPTORCOUNT, static_cast<Effector>(effectorI));
			int id = Nodes.size();
			Nodes.push_back(std::shared_ptr<NeuronNode>(node));
			node->id = id;
			vals->operator[](id) = 0.5;
		}

		prevVals->clear();
		for (auto kv : *vals) {
			prevVals->operator[](kv.first) = kv.second;
		}
	}

	void NeuralMap::FormMapFromGenome(std::vector<Gene> Genome) {
		// Form a connection between neurons for each gene
		for (Gene& gene : Genome) {
			std::shared_ptr<NeuronNode> sink = Nodes[gene.sinkId];
			std::shared_ptr<NeuronNode> source = Nodes[gene.sourceId];

			// Give the dest neuron the source neuron's id as well as the connection weight
			sink->in.push_back(gene.sourceId);
			sink->inWeights.push_back(gene.weightFloat());

			// Tell the source and dest neuron that they are connected on each side
			source->connected = true;
			sink->connectedIn = true;
		}
	}

	void NeuralMap::Execute() {
		if (owner == 0) {
			LOG_ERROR("Tried to execute a NeuralMap with no owner Node");
			return;
		}

		// For each effector, trace back down the neuron tree (using cached values to prevent recalculation) (inputs are stored in the cache before hand)
		for (auto Node : Nodes) {
			if (!Node->type == NeuronType::EFFECTOR) {
				continue;
			}
			
			if (!Node->connectedIn) {
				continue;
			}

			std::vector<int> visitedChain; // this is used to prevent infinite loops when evaluating the map
			float val = Node->eval(Nodes, vals, prevVals, visitedChain);
			vals->operator[](Node->id) = val;

			if (Queues::EffectorQueue.find(owner) != Queues::EffectorQueue.end()) {
				Queues::EffectorQueue[owner].push_back(Queues::ActionEvent(Node->effectorType, val));
			}
			else {
				Queues::EffectorQueue.insert(std::make_pair(owner, std::vector<Queues::ActionEvent>({ Queues::ActionEvent(Node->effectorType, val) })));
			}
		}
	}

}