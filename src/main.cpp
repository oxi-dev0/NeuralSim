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

#include "Tools/SysIntegration.h"

#include "Classes/Utils.h"
#include "Classes/Globals.h"
#include "Classes/Generation.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

// ---- CONFIG ----
int tileSize = 6;

void RenderFrame(sf::RenderTexture& texture, uint16_t gen, int frame) {
    Debug::Timer frametmr;
 
    texture.clear();

    for (int c = 0; c < Globals::GlobalGrid.cells.size(); c++) {
        sf::CircleShape shape(tileSize / 2);
        Vector2D pos = Globals::GlobalGrid.cells[c]->pos * Utils::GlobalConfig.tileSize;
        shape.setPosition(sf::Vector2f(pos.x, pos.y));
        texture.draw(shape);
    }
    
    texture.display();

    char s[25];
    sprintf(s, "%04d", frame);
    std::string framnum(s);

    std::stringstream filepipe = std::stringstream();
    filepipe << "temp/framerender-" << gen << "-" << framnum << ".png";
    std::string filename = filepipe.str();


    if (!std::filesystem::is_directory("temp") || !std::filesystem::exists("temp")) {
        std::filesystem::create_directory("temp");
    }

    if (!texture.getTexture().copyToImage().saveToFile(filename)) {
        LOG_ERROR("Failed to save the render to {0}", filename);
    }
    else {
        LOG_TRACE("Rendered frame {2} to {0} in {1}s", filename, frametmr.Elapsed(), framnum);
    }
}

void TestUpdate() {
    for (Cell* cellP : Globals::GlobalGrid.cells) {
        Cell cell = *cellP;
        if (cell.pos.x < Utils::GlobalConfig.sizeX && cell.pos.y < Utils::GlobalConfig.sizeY) {
            cell.pos = cell.pos + Vector2D(1, 0);
            uint16_t id = Globals::GlobalGrid.at(cell.pos - Vector2D(1, 0));
            if (cell.pos.x >= Utils::GlobalConfig.sizeX || cell.pos.y >= Utils::GlobalConfig.sizeY) {
                Globals::GlobalGrid.set(cell.pos - Vector2D(1, 0), 0);
            }
            else {
                Globals::GlobalGrid.set(cell.pos - Vector2D(1, 0), 0);
                Globals::GlobalGrid.set(cell.pos, id);
            }
        }
    }
}

void ProduceVideo(uint16_t gen, int fps) {
    if (!std::filesystem::is_directory("renders") || !std::filesystem::exists("renders")) {
        std::filesystem::create_directory("renders");
    }

    std::stringstream outpipe = std::stringstream();
    outpipe << "renders/generation" << gen << ".mp4";
    std::string outfile(outpipe.str());

    LOG_TRACE("Writing Generation {0}'s frames to {1}", gen, outfile);

    std::stringstream cmdpipe = std::stringstream();
    cmdpipe << "\"ffmpeg.exe\"" << " -framerate " << fps << " -i" << " temp/framerender-" << gen << "-%04d.png " << outfile;
    std::string cmd = cmdpipe.str();

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(cmd);

    if (std::filesystem::exists(outfile)) {
        std::filesystem::remove(outfile);
    }

    LOG_TRACE("FFMPEG: {0}", ExecCmd(wide.c_str()));

    // Remove old rendered frames
    std::filesystem::remove_all("temp");

    LOG_INFO("Successfully rendered Generation {0} to {1}", gen, outfile);
}

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

    Tools::Config::InitFromArgv(argc, argv);
    Utils::Init();
    Globals::Init();

    LOG_INFO("Initialised Arguments, Utils and Globals in {0}s", inittmr.Elapsed());

    Debug::Timer debugtmr;
    // debug initialise grid
    for (int i = 0; i < Utils::GlobalConfig.population; i++) {
        bool valid = false;
        while (!valid) {
            Vector2D pos = Vector2D(Utils::RandRange(0, Utils::GlobalConfig.sizeX - 1), Utils::RandRange(0, Utils::GlobalConfig.sizeY - 1));
            Cell newCell = Cell(pos);
            if (Globals::GlobalGrid.isEmptyAt(pos)) {
                valid = true;
                Globals::GlobalGrid.NewCell(&newCell);
                // map needs cell id
                newCell.InitaliseMap();

                NeuralNet::Genome TESTGENOME({ NeuralNet::Gene(1,5,1) }); // 1 connection genome should make it so cells move right if over halfway line

                // create neural map
                newCell.InitialiseGenome(TESTGENOME);
            }
            else {
                uint16_t val = Globals::GlobalGrid.columns[pos.x][pos.y];
            }
        }
    }
    LOG_INFO("Spawned generation in {0}", debugtmr.Elapsed());

    sf::RenderTexture texture;
    if (!texture.create(Utils::GlobalConfig.sizeX * tileSize, Utils::GlobalConfig.sizeY * tileSize))
    {
        LOG_ERROR("Could not render the current generation");
    }
    else {
        int f = 0;
        for (int f = 0; f < 20; f++) {
            SimulationStep();
            RenderFrame(texture, Utils::GlobalSimData.currentGen, Utils::GlobalSimData.currentStep);
        }
        ProduceVideo(Utils::GlobalSimData.currentGen, Utils::GlobalConfig.framerate);
    }

    return 0;
}
