#include <iostream>

#include <SFML/Graphics.hpp>

#include "Tools/Log.h"
#include "Tools/Timer.h"
#include "Tools/Config.h"
#include "Tools/Random.h"

#include "Classes/Globals.h"

// ---- CONFIG ----
int tileSize = 6;

void RenderGeneration() {
    sf::RenderTexture texture;
    if (!texture.create(Globals::GlobalConfig.sizeX * tileSize, Globals::GlobalConfig.sizeY * tileSize))
    {
        LOG_ERROR("Could not render the current generation");
    }
    texture.clear();

    sf::CircleShape shape(tileSize/2);
    shape.setPosition(vec2);
    texture.draw(shape);
    
    texture.display();

    if (!texture.getTexture().copyToImage().saveToFile("render.png")) {
        LOG_ERROR("Failed to save the render to 'render.png'");
    }
}

void usage(char* progName)
{
    std::cout << progName << " [options]" << std::endl <<
        "Options:" << std::endl <<
        "-h | --help                   Print this help" << std::endl <<
        "-x | --sizeX       [int]      Specify the grid X size" << std::endl <<
        "-y | --sizeY       [int]      Specify the grid Y size" << std::endl <<
        "-p | --population  [int]      Specify the population size per generation" << std::endl;
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

    Debug::Timer atmr;
    Tools::Config::InitFromArgv(argc, argv);
    Globals::Init();
    LOG_INFO("Parsed console arguments in {0}s", atmr.Elapsed());

    Debug::Timer rtmr;
    RenderGeneration();
    LOG_INFO("Rendered test image in {0}s", atmr.Elapsed());

    return 0;
}
