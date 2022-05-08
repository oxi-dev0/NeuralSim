#include "DebugTypes.h"

namespace NeuralNet {
	std::tuple<std::string, std::string> receptorName(Receptor receptor) {
		switch (receptor) {
		case Receptor::AGE: return std::tuple<std::string, std::string>("AGE", "Age"); break;
		case Receptor::LOC_X: return std::tuple<std::string, std::string>("Lx", "X Loc"); break;
		case Receptor::LOC_Y: return std::tuple<std::string, std::string>("Ly", "Y Loc"); break;
		case Receptor::OSC: return std::tuple<std::string, std::string>("OSC", "Oscillator"); break;
		case Receptor::RANDOM: return std::tuple<std::string, std::string>("RND", "Random"); break;
		}
	}
	
	std::tuple<std::string, std::string> effectorName(Effector effector) {
		switch (effector) {
		case Effector::MOVE_X: return std::tuple<std::string, std::string>("Mx", "Move X"); break;
		case Effector::MOVE_Y: return std::tuple<std::string, std::string>("My", "Move Y"); break;
		case Effector::MOVE_FB: return std::tuple<std::string, std::string>("Mfb", "Move Forward/Backward"); break;
		case Effector::MOVE_LR: return std::tuple<std::string, std::string>("Mlr", "Move Left/Right"); break;
		case Effector::MOVE_RANDOM: return std::tuple<std::string, std::string>("Mrn", "Move Random"); break;
		case Effector::MOVE_NORTH: return std::tuple<std::string, std::string>("McN", "Move Compass North"); break;
		case Effector::MOVE_EAST: return std::tuple<std::string, std::string>("McE", "Move Compass East"); break;
		case Effector::MOVE_SOUTH: return std::tuple<std::string, std::string>("McS", "Move Compass South"); break;
		case Effector::MOVE_WEST: return std::tuple<std::string, std::string>("McW", "Move Compass West"); break;
		}
	}
}