#include "Types.h"

namespace NeuralNet {
	float NeuronNode::eval(const std::vector<std::shared_ptr<NeuronNode>> Nodes, std::unordered_map<int, float>* vals, std::unordered_map<int, float>* prevVals, std::vector<int> visited) {
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
				if (std::find(visited.begin(), visited.end(), nodeIndex) != visited.end()) {
					// The source node has already been visited in this eval chain, use its cached value to prevent inf loop
					float inVal = prevVals->operator[](nodeIndex);
					float weight = inWeights[i];
					sum += (inVal * weight);
					i++;
					continue;
				}

				// If the value of the source is not cached, calculate it
				visited.push_back(id); // add self to visited chain
				float inVal = Nodes[nodeIndex]->eval(Nodes, vals, prevVals, visited);
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

		// return tanh(sum)
		const float e = 2.718f;
		const float ePow = pow(e, 2 * sum);
		return (ePow-1)/(ePow+1);
	}
}