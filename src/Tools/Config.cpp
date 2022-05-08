#include "Config.h"

#define MAX_ALLOWED_BUFFER 100

namespace Tools {
	std::vector<std::tuple<char*, char*>> ArgvLongPairs(int argc, char** argv) {
		char** argvC = argv;
		int argcC = argc;

		// Remove first item from argvC
		argvC++;
		argcC--;

		std::unordered_map<std::string, std::string> shortMap = {
			{ "x", "sizeX"},
			{ "y", "sizeY" },
			{ "p", "population" },
			{ "fr", "framerate" },
			{ "in", "internalneurons"},
			{ "spg", "stepspergen"},
			{ "c", "config"}
		};

		std::vector<std::tuple<char*, char*>> pairs;
		std::tuple<char*, char*> currentPair;
		while (argcC > 0) {
			char* str = *argvC;

			// If first item in pair
			if (argcC % 2 == 0) {
				bool shrt = false;

				if (str[1] == '-') {
					str += 2;
				}
				else {
					str += 1;
					shrt = true;
				}

				currentPair = std::tuple<char*, char*>();

				// Transform short to long param
				if (shrt) {
					std::string strV(str);
					std::string lng = shortMap[strV];
					char* strC = new char[strlen(str)];
					strcpy(strC, lng.c_str());
					std::get<0>(currentPair) = strC;
				}
				else {
					std::get<0>(currentPair) = str;
				}
			}
			else {
				std::get<1>(currentPair) = str;
				pairs.push_back(currentPair);
			}

			argvC++;
			argcC--;
		}

		return pairs;
	}

	Config Config::InitFromArgv(int argc, char** argv) {
		std::vector<std::tuple<char*, char*>> pairs = ArgvLongPairs(argc, argv);

		std::string configFile = "config.ini";

		int i = 0;
		for (std::tuple<char*, char*>& pair : pairs) {
			std::string key(std::get<0>(pair));
			std::string val(std::get<1>(pair));

			if (key == "config") { configFile = val; }

			i++;
		}

		return InitFromIni(configFile);
	}

	Config Config::InitFromIni(const std::string iniS) {
		mINI::INIFile file(iniS);
		mINI::INIStructure ini;

		file.read(ini);

		Config newConfig;
		newConfig.sizeX = std::stoi(ini["scene"]["sizeX"]);
		newConfig.sizeY = std::stoi(ini["scene"]["sizeY"]);
		newConfig.tileSize = std::stoi(ini["scene"]["tileSize"]);

		newConfig.framerate = ini["render"]["framerate"];
		newConfig.gensBetweenRender = std::stoi(ini["render"]["gensBetweenRender"]);

		newConfig.population = std::stoi(ini["sim"]["population"]);
		newConfig.internalNeuronCount = std::stoi(ini["sim"]["internalNeuronCount"]);
		newConfig.genomeLength = std::stoi(ini["sim"]["genomeLength"]);
		newConfig.stepsPerGen = std::stoi(ini["sim"]["stepsPerGen"]);
		newConfig.mutationChance = std::stof(ini["sim"]["mutationChance"]);

		return newConfig;
	}
}