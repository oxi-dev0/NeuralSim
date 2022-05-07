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
		Config(int sizeX_, int sizeY_, int tileSize_, int population_, int framerate_) { sizeX = sizeX_; sizeY = sizeY_; tileSize = tileSize_; population = population_; framerate = framerate_; }
		Config() { sizeX = 128; sizeY = 128; tileSize = 6; population = 1000; framerate = 4; }

		static void InitFromArgv(int argc, char **argv);

	public:
		int sizeX;
		int sizeY;
		int tileSize;
		int population;
		int framerate;
	};

	inline Config InitConfig;
}