#pragma once
#ifndef CONFIG
#define CONFIG

#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>

#include "Log.h"

namespace Tools {
	struct Config
	{
	public:
		Config(int sizeX_, int sizeY_, int population_) { sizeX = sizeX_; sizeY = sizeY_; population = population_; }
		Config() { sizeX = 0; sizeY = 0; population = 0; }

		static void InitFromArgv(int argc, char **argv);

	public:
		int sizeX;
		int sizeY;
		int population;
	};

	inline Config InitConfig;
}

#endif