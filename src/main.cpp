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

#include "Visualise/Visualise.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

void usage(char* progName)
{
    std::cout << progName << " [options]" << std::endl <<
        "Options:" << std::endl <<
        "-h   | --help                          Print this help" << std::endl <<
        "[MODES]" << std::endl <<
        "-s   | --simulate                      Simulate generations of cells [DEFAULT]" << std::endl <<
        "-v   | --visualise       [*.nm]        Visualise an exported neural map file" << std::endl <<
        "[OTHER]" << std::endl <<
        "-c   | --config          [*.ini]       Specify the simulation configuration file [DEFAULT: config/config.ini]" << std::endl <<
        "-sc  | --survival        [*.shape]     Specify the survival shape file [DEFAULT: config/survival.shape]" << std::endl <<
        "-k   | --kconst          [int]         Set the Fruchterman-Reingold constant [DEFAULT: 400]" << std::endl;
}

void sim(sf::RenderTexture& texture) {
    NewGeneration(std::vector<NeuralNet::Genome>(), true);
    while (true) {
        SimulationStep(texture);
    }
}

int main(int argc, char** argv)
{
#ifdef DIST
    FreeConsole();
#else
    Debug::Log::Init();
#endif

    Tools::ProgramMode mode = Tools::Config::ModeFromArgv(argc, argv);
    if (mode == Tools::ProgramMode::HELP) { usage(argv[0]); return 0; }

    LOG_INFO("Started Engine");

    Debug::Timer inittmr;
    Tools::Config config = Tools::Config::InitFromArgv(argc, argv);

    Utils::Init();
    Globals::Init();
    Utils::GlobalConfig = config;

    LOG_INFO("Initialised Arguments, Utils and Globals in {0}s", inittmr.Elapsed());

    Debug::Timer parseTimer;
    Shapes::SurvivalConfig = Shapes::ParseShapeFile(Utils::GlobalConfig.survivalconfigfile);
    LOG_INFO("Parsed survival shape file in {0}s", parseTimer.Elapsed());

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderTexture texture;
    if (!texture.create(Utils::GlobalConfig.sizeX * Utils::GlobalConfig.tileSize, Utils::GlobalConfig.sizeY * Utils::GlobalConfig.tileSize, settings))
    {
        LOG_CRITICAL("Could not initialise the render texture");
    }
    else {
        switch (mode) {
        case Tools::ProgramMode::SIMULATE: sim(texture); break;
        case Tools::ProgramMode::VISUALISE: Visualisation::VisualiseNeuralMap(texture, Utils::GlobalConfig.neuralmap); break;
        }
    }

    return 0;
}
