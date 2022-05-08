#pragma once

#include "../Tools/Random.h"
#include "../Tools/Config.h"
#include "../Tools/SimData.h"

namespace Utils {
	extern Random::RandomGenerator RandRange;
	extern Tools::Config GlobalConfig;
	extern Sim::SimData GlobalSimData;
	extern void Init();
}
