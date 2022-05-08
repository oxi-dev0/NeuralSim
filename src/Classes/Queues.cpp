#include "Queues.h"

namespace Queues {
	std::unordered_map<int, std::vector<std::shared_ptr<ActionEvent>>> EffectorQueue;
	void Init() {
		EffectorQueue = std::unordered_map<int, std::vector<std::shared_ptr<ActionEvent>>>();
	}
}