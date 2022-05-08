#include "Queues.h"

namespace Queues {
	std::vector<ActionEvent> EffectorQueue;
	void Init() {
		EffectorQueue = std::vector<ActionEvent>();
	}
}