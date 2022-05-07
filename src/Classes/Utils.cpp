#include "Utils.h"

namespace Utils {
	Random::RandomGenerator RandRange;
	Tools::Config GlobalConfig;
	void Init() {
		RandRange = Random::RandomGenerator();
		GlobalConfig = Tools::InitConfig;
	}
}