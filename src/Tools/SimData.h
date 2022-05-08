#pragma once

namespace Sim {
	class SimData {
	public:
		int currentStep;
		int currentGen;

		SimData() { currentStep = 0; currentGen = 0; }
		SimData(int cs, int cg) { currentStep = cs; currentGen = cg; }
	};
}
