#pragma once

#include <cstdint>
#include "../Utils.h"
#include "Types.h"

namespace NeuralNet {
	extern std::tuple<std::string, std::string> receptorName(Receptor receptor);
	extern std::tuple<std::string, std::string> effectorName(Effector effector);
}