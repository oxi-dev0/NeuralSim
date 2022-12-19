#include "Types.h"

namespace NeuralNet {
	
	// Evaluation works by starting at each output node, call a function where for each input of the node, it then determines the input value:
	//	-	If the input node is the original node, it uses the cached value from the previous step
	//	-	If the input node has already been calculated then it uses that calculated value
	//	-	If the input node has already been visited in the evaluation chain, then it uses the cached value from the previous step
	//	-	If all of these were false, then it calls the evaluation function on this input node
	//	Then, it multiplies the value by the input weight, and adds it to the output sum. After all the inputs are added to the sum, it returns tanh(sum).

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