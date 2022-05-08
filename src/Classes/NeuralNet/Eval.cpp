#include "Types.h"

namespace NeuralNet {
	float NeuronNode::eval(std::vector<std::shared_ptr<NeuronNode>> Nodes, std::unordered_map<int, float>* vals, std::unordered_map<int, float>* prevVals) {
		float sum = 0;
		// For every connection into the neuron
		int i = 0;
		for (int nodeIndex : in) {
			if (nodeIndex == id) {
				// self reference, use previous step cached value
				float inVal = prevVals->operator[](nodeIndex);
				float weight = inWeights[i];
				sum += (inVal * weight);
				i++;
				continue;
			}

			if (!vals->contains(nodeIndex)) {
				auto inCopy = Nodes[nodeIndex]->in;
				if (std::find(inCopy.begin(), inCopy.end(), id) != inCopy.end()) {
					// The source node has a reference back to this node, meaning it will loop forever; therefore use the previous cached value
					float inVal = prevVals->operator[](nodeIndex);
					float weight = inWeights[i];
					sum += (inVal * weight);
					i++;
					continue;
				}
				// If the value of the source is not cached, calculate it
				float inVal = Nodes[nodeIndex]->eval(Nodes, vals, prevVals);
				vals->operator[](nodeIndex) = inVal;
				float weight = inWeights[i];
				sum += (inVal * weight);
			}
			else {
				// The value of the source is already cached
				float inVal = vals->operator[](nodeIndex);
				float weight = inWeights[i];
				sum += (inVal * weight);
			}

			i++;
		}

		return (std::tanh(sum));
	}
}