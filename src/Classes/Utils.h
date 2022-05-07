#pragma once

#include "../Tools/Random.h"
#include "../Tools/Config.h"

namespace Utils {
	extern Random::RandomGenerator RandRange;
	extern Tools::Config GlobalConfig;
	extern void Init();
}
