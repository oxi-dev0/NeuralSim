#include "Globals.h"

namespace Globals {
	Random::RandomGenerator RandRange;
	Tools::Config GlobalConfig;
	void Init() {
		RandRange = Random::RandomGenerator();
		GlobalConfig = Tools::InitConfig;
	}
}