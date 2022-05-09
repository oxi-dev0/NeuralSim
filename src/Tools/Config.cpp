#include "Config.h"

#define MAX_ALLOWED_BUFFER 100

namespace Tools {
	std::unordered_map<std::string, std::string> ArgvLongPairs(int argc, char** argv) {
		char** argvC = argv;
		int argcC = argc;

		if (argcC < 2) {
			return std::unordered_map<std::string, std::string>();
		}

		// Remove exe item from argvC
		argvC++;
		argcC--;

		std::unordered_map<std::string, std::string> shortMap = {
			{ "h", "help" },
			{ "s", "simulate" },
			{ "v", "visualise" },
			{ "c", "config"}
		};

		std::unordered_map<std::string, bool> valMap = {
			// name, takes value
			{ "help", false },
			{ "simulate", false },
			{ "visualise", true },
			{ "config", true }
		};

		std::unordered_map<std::string, std::string> map;
		std::string current = "";
		bool reqVal = false;
		while (argcC > 0) {
			std::string str(*argvC);

			if (!reqVal) {
				bool shrt = false;

				if (str[1] == '-') {
					str = str.substr(2);
				}
				else {
					str = str.substr(1);
					shrt = true;
				}

				// Transform short to long param
				if (shrt) {
					std::string lng = shortMap[str];
					map[lng] = "NULL";
					current = lng;
					reqVal = valMap[lng];
				}
				else {
					map[str] = "NULL";
					current = str;
					reqVal = valMap[str];
				}
			}
			else {
				map[current] = str;
				reqVal = false;
			}

			argvC++;
			argcC--;
		}

		return map;
	}

	Config Config::InitFromArgv(int argc, char** argv) {
		std::unordered_map<std::string, std::string> map = ArgvLongPairs(argc, argv);

		std::string configFile = "config/config.ini";
		std::string nmFile = "";

		for (auto& kv : map) {
			std::string key(kv.first);
			std::string val(kv.second);

			if (key == "config") { configFile = val; }
			if (key == "visualise") { nmFile = val; }
		}

		if (nmFile != "") {
			return Config(1000, 1000, 1, 0, 0, 0, 0, 0, 0, "", nmFile);
		}

		return InitFromIni(configFile);
	}

	ProgramMode Config::ModeFromArgv(int argc, char** argv) {
		std::unordered_map<std::string, std::string> map = ArgvLongPairs(argc, argv);

		ProgramMode mode = ProgramMode::SIMULATE;

		for (auto& kv : map) {
			std::string key(kv.first);

			if (key == "help") { mode = ProgramMode::HELP; }
			if (key == "simulate") { mode = ProgramMode::SIMULATE; }
			if (key == "visualise") { mode = ProgramMode::VISUALISE;  }
		}

		return mode;
	}

	Config Config::InitFromIni(const std::string iniS) {
		if (!std::filesystem::exists(iniS)) { LOG_CRITICAL("Could not load configuration file ({0})", iniS); std::terminate(); }

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