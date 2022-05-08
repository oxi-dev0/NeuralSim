#pragma once

#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>

#define MINI_CASE_SENSITIVE
#include "ini.h"

#include "Log.h"

namespace Tools {
	struct Config
	{
	public:
		Config(int sizeX_, int sizeY_, int tileSize_, int population_, int internalNeuronCount_, int genomeLength_, int mutationChance_, int stepsPerGen_, int gensBetweenRender_, std::string framerate_) { sizeX = sizeX_; sizeY = sizeY_; tileSize = tileSize_; population = population_; internalNeuronCount = internalNeuronCount_; genomeLength = genomeLength_; mutationChance = mutationChance_; stepsPerGen = stepsPerGen_; gensBetweenRender = gensBetweenRender_; framerate = framerate_; }
		Config() { sizeX = 128; sizeY = 128; tileSize = 6; population = 1000; internalNeuronCount = 2; mutationChance = 0.001f; genomeLength = 4; stepsPerGen = 350; gensBetweenRender = 100, framerate = "350/10"; }

		static Config InitFromArgv(int argc, char **argv);
		static Config InitFromIni(const std::string ini);

	public:
		int sizeX;
		int sizeY;
		int tileSize;
		int population;
		int internalNeuronCount;
		float mutationChance;
		int genomeLength;
		int stepsPerGen;
		int gensBetweenRender;
		std::string framerate;
	};
}