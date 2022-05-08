#pragma once

#include "Globals.h"
#include "Utils.h"
#include "Queues.h"
#include "../Tools/Log.h"

extern void ProcessEffectorQueue();

inline void SimulationStep() {
	Utils::GlobalSimData.currentStep++;
	LOG_TRACE("Current Step: {0}", Utils::GlobalSimData.currentStep);

	for (Cell* cell : Globals::GlobalGrid.cells) {
		cell->Step();
	}

	ProcessEffectorQueue();
	if (Utils::GlobalSimData.currentStep > Utils::GlobalConfig.stepsPerGen) {
		Utils::GlobalSimData.currentGen++;
		Utils::GlobalSimData.currentStep = 0;
		// NEW GEN
	}
}

