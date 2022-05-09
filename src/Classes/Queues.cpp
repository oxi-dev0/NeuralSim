#include "Queues.h"

namespace Queues {
	std::unordered_map<int, std::vector<ActionEvent>> EffectorQueue;
	void Init() {
		EffectorQueue = std::unordered_map<int, std::vector<ActionEvent>>();
	}
}