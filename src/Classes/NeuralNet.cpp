#include "NeuralNet.h"

namespace NeuralNet {

	void NeuralMap::InitialiseNodesList() {
		Nodes.clear();
		vals.clear();

		for (int receptorI = Receptor::LOC_X; receptorI != Receptor::RECEPTORCOUNT; receptorI++)
		{
			NeuronNode node(NeuronType::RECEPTOR, -1, static_cast<Receptor>(receptorI), Effector::ACTIONCOUNT);
			int id = Nodes.size();
			Nodes.push_back(&node);
			node.id = id;
			vals[id] = 0.5;
		}

		for (int c = 0; c < Utils::GlobalConfig.internalNeuronCount; c++)
		{
			NeuronNode node(NeuronType::INTERNAL, -1, Receptor::RECEPTORCOUNT, Effector::ACTIONCOUNT);
			int id = Nodes.size();
			Nodes.push_back(&node);
			node.id = id;
			vals[id] = 0.5;
		}

		for (int effectorI = Effector::MOVE_X; effectorI != Effector::ACTIONCOUNT; effectorI++)
		{
			NeuronNode node(NeuronType::EFFECTOR, -1, Receptor::RECEPTORCOUNT, static_cast<Effector>(effectorI));
			int id = Nodes.size();
			Nodes.push_back(&node);
			node.id = id;
			vals[id] = 0.5;
		}

		prevVals.clear();
		prevVals = vals;
	}

	void NeuralMap::FormMapFromGenome(std::vector<Gene> Genome) {
		for (Gene& gene : Genome) {
			NeuronNode* sink = Nodes[gene.sinkId];
			NeuronNode* source = Nodes[gene.sourceId];

			sink->in.push_back(gene.sourceId);
			LOG_TRACE(gene.weightFloat());
			sink->inWeights.push_back(gene.weightFloat());
			source->connected = true;
			sink->connectedIn = true;
		}
	}

	void NeuralMap::Execute() {
		if (owner == 0) {
			LOG_ERROR("Tried to execute a NeuralMap with no owner Node");
			return;
		}
		
		prevVals.clear();
		prevVals = vals;
		vals.clear();

		// For each effector, trace back down the tree (using cached values to prevent recalculation) (inputs are stored in the cache before hand)
		for (NeuronNode* Node : Nodes) {
			if (!Node->type == NeuronType::EFFECTOR) {
				continue;
			}
			
			if (!Node->connectedIn) {
				continue;
			}

			float val = Node->eval(Nodes, vals, prevVals);
			vals[Node->id] = val;

			if (std::abs(val) > 0.5) {
				Queues::EffectorQueue.push_back(Queues::ActionEvent(owner, Node->effectorType, val));
			}
		}
	}

}