#pragma once

#include <vector>

#include "NeuralNet/Types.h"

namespace Queues {
	struct ActionEvent {
		NeuralNet::Effector action;
		float amnt;

		ActionEvent(NeuralNet::Effector action_, float amnt_) {
			action = action_;
			amnt = amnt_;
		}
	};

	extern std::unordered_map<int, std::vector<std::shared_ptr<ActionEvent>>> EffectorQueue;
	extern void Init();
}