#pragma warning( disable : 4244 4267 4996 )

#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "Tools/Log.h"
#include "Tools/Timer.h"
#include "Tools/Config.h"
#include "Tools/Random.h"

#include "Classes/Utils.h"
#include "Classes/Globals.h"
#include "Classes/Generation.h"

#include "Classes/Output.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

void usage(char* progName)
{
    std::cout << progName << " [options]" << std::endl <<
        "Options:" << std::endl <<
        "-h   | --help                          Print this help" << std::endl <<
        "-x   | --sizeX              [int]      Specify the grid X size" << std::endl <<
        "-y   | --sizeY              [int]      Specify the grid Y size" << std::endl <<
        "-p   | --population         [int]      Specify the population size per generation" << std::endl <<
        "-in  | --internalneurons    [int]      The number of internal neurons each cell has" << std::endl <<
        "-fr  | --framerate          [int]      The framerate of rendered generations" << std::endl <<
        "-spg | --stepspergen        [int]      The number of simulator steps per generation" << std::endl;
}

int main(int argc, char** argv)
{
    std::string arg(argv[1]);
    if (arg == "--help" || arg == "-h") {
        usage(argv[0]);
        return 0;
    }

#ifdef DIST
    FreeConsole();
#else
    Debug::Log::Init();
#endif

    LOG_INFO("Started Engine");

    Debug::Timer inittmr;

    Tools::Config config = Tools::Config::InitFromArgv(argc, argv);
    Utils::Init();
    Globals::Init();
    Utils::GlobalConfig = config;

    LOG_INFO("Initialised Arguments, Utils and Globals in {0}s", inittmr.Elapsed());

    sf::RenderTexture texture;
    if (!texture.create(Utils::GlobalConfig.sizeX * Utils::GlobalConfig.tileSize, Utils::GlobalConfig.sizeY * Utils::GlobalConfig.tileSize))
    {
        LOG_CRITICAL("Could not initialise the render texture");
    }
    else {
        NewGeneration(std::vector<NeuralNet::Genome>(), true);
        while (true) {
            SimulationStep(texture);
        }
    }

    return 0;
}
