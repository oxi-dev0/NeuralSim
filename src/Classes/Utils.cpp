#include "Utils.h"

namespace Utils {
	Random::RandomGenerator RandRange;
	Tools::Config GlobalConfig;
	Sim::SimData GlobalSimData;
	void Init() {
		RandRange = Random::RandomGenerator();
		GlobalConfig = Tools::Config();
		GlobalSimData = Sim::SimData(0,0,false);
	}
}