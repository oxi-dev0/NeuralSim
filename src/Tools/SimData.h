#pragma once

namespace Sim {
	class SimData {
	public:
		int currentStep;
		int currentGen;
		bool renderGen;

		SimData() { currentStep = 0; currentGen = 0; renderGen = false; }
		SimData(int cs, int cg, bool rg) { currentStep = cs; currentGen = cg; renderGen = rg; }
	};
}
