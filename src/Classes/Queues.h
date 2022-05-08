#pragma once

#include <vector>

#include "NeuralNet/Types.h"

namespace Queues {
	struct ActionEvent {
		int owner;
		NeuralNet::Effector action;
		float amnt;

		ActionEvent(int owner_, NeuralNet::Effector action_, float amnt_) {
			owner = owner_;
			action = action_;
			amnt = amnt_;
		}
	};

	extern std::vector<ActionEvent> EffectorQueue;
	extern void Init();
}