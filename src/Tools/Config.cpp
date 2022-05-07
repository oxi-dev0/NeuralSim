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
			{ "fr", "framerate" }
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

	void Config::InitFromArgv(int argc, char** argv) {
		std::vector<std::tuple<char*, char*>> pairs = ArgvLongPairs(argc, argv);

		Config newConfig;

#ifdef DEBUG
		LOG_TRACE("[COMMAND ARGS]");
#endif

		int i = 0;
		for (std::tuple<char*, char*>& pair : pairs) {
#ifdef DEBUG
			LOG_TRACE("	ARG PAIR[{0}]: {1} = {2}", i, std::get<0>(pair), std::get<1>(pair));
#endif
			std::string key(std::get<0>(pair));
			std::string val(std::get<1>(pair));
			
			if (key == "sizeX") { newConfig.sizeX = std::stoi(val); }
			if (key == "sizeY") { newConfig.sizeY = std::stoi(val); }
			if (key == "population") { newConfig.population = std::stoi(val); }
			if (key == "framerate") { newConfig.framerate = std::stoi(val); }

			i++;
		}

		InitConfig = newConfig;
	}
}