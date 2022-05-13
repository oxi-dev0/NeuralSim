#pragma once

#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>
#include <filesystem>

#define MINI_CASE_SENSITIVE
#include "ini.h"

#include "Log.h"

namespace Tools {
	enum ProgramMode {
		HELP,
		SIMULATE,
		VISUALISE
	};

	struct Config
	{
	public:
		Config(int sizeX_, int sizeY_, int tileSize_, int population_, int internalNeuronCount_, int genomeLength_, float mutationChance_, int stepsPerGen_, int gensBetweenRender_, std::string framerate_, std::string neuralmap_, int kconst_, std::string survival_) { sizeX = sizeX_; sizeY = sizeY_; tileSize = tileSize_; population = population_; internalNeuronCount = internalNeuronCount_; genomeLength = genomeLength_; mutationChance = mutationChance_; stepsPerGen = stepsPerGen_; gensBetweenRender = gensBetweenRender_; framerate = framerate_; neuralmap = neuralmap_; kconst = kconst_; survivalconfigfile = survival_; }
		Config() { sizeX = 128; sizeY = 128; tileSize = 6; population = 1000; internalNeuronCount = 2; mutationChance = 0.001f; genomeLength = 4; stepsPerGen = 350; gensBetweenRender = 100, framerate = "350/10"; neuralmap = ""; kconst = 400; survivalconfigfile = "config/survival.shape"; }

		static Config InitFromArgv(int argc, char **argv);
		static Config InitFromIni(const std::string ini, Config newConfig);

		static ProgramMode ModeFromArgv(int argc, char** argv);

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
		int kconst;
		std::string framerate;

		std::string neuralmap;
		std::string survivalconfigfile;
	};
}