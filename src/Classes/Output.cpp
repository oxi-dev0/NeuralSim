#include "Output.h"

void RenderFrame(sf::RenderTexture& texture, uint16_t gen, int frame) {
    Debug::Timer frametmr;

    texture.clear();

    for (int c = 0; c < Globals::GlobalGrid.cells.size(); c++) {
        if (Globals::GlobalGrid.cells[c] == nullptr) { continue; }
        sf::CircleShape shape(Utils::GlobalConfig.tileSize / 2);
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

void ProduceVideo(uint16_t gen, std::string fps) {
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


// Template is at reference/neuralmapexporttemplate.txt
void ExportNeuralMap(std::shared_ptr<NeuralNet::NeuralMap> map) {
    if (!std::filesystem::is_directory("out") || !std::filesystem::exists("out")) {
        std::filesystem::create_directory("out");
    }
    std::stringstream filename;
    filename << "out/" << Utils::GlobalSimData.currentGen << "-" << map->owner << "-NeuralMap.txt";
    std::string fn(filename.str());

    std::ofstream f(fn);
    int n = 0;
    for (auto& node : map->Nodes) {
        if (node == nullptr) { continue; }
        f << n << "-";
        if (node->type == NeuralNet::NeuronType::RECEPTOR) {
            f << "r:";
            std::string nName = std::get<0>(NeuralNet::receptorName(node->receptorType));
            f << nName;
        }
        if (node->type == NeuralNet::NeuronType::INTERNAL) {
            f << "i";
        }
        if (node->type == NeuralNet::NeuronType::EFFECTOR) {
            f << "e:";
            std::string nName = std::get<0>(NeuralNet::effectorName(node->effectorType));
            f << nName;
        }

        int i = 0;
        for (int in : node->in) {
            float weight = node->inWeights[i];
            f << "{" << in << "@" << (int)weight * 80192 << "}";
            i++;
        }

        f << "\n";

        n++;
    }

    f.close();
    LOG_INFO("Wrote Cell {0}'s Neural Map to {1} successfully", map->owner, fn);
}