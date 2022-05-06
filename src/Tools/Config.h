#pragma once

#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>

#include "Log.h"

namespace Tools {
	struct Config
	{
	public:
		Config(int sizeX_, int sizeY_, int tileSize_, int population_) { sizeX = sizeX_; sizeY = sizeY_; tileSize = tileSize_; population = population_; }
		Config() { sizeX = 128; sizeY = 128; tileSize = 6; population = 1000; }

		static void InitFromArgv(int argc, char **argv);

	public:
		int sizeX;
		int sizeY;
		int tileSize;
		int population;
	};

	inline Config InitConfig;
}