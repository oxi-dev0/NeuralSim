#include "Types.h"

namespace NeuralNet {
	float NeuronNode::eval(std::vector<NeuronNode*> Nodes, std::unordered_map<int, float>& vals, std::unordered_map<int, float>& prevVals) {
		float sum = 0;
		for (int nodeIndex : in) {
			if (!vals.contains(nodeIndex)) {
				float inVal = Nodes[nodeIndex]->eval(Nodes, vals, prevVals);
				vals[nodeIndex] = inVal;
				float weight = inWeights[nodeIndex];
				sum += (inVal * weight);
			}
			else {
				float inVal = vals[id];
				if (nodeIndex == id) {
					// self reference
					float inVal = prevVals[id];
				}
				float weight = inWeights[id];
				sum += (inVal * weight);
			}
		}

		return (std::tanh(sum) + 1) / 2;
	}
}