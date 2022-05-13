#pragma once

#include <sstream>
#include <locale>
#include <codecvt>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "../Tools/Timer.h"
#include "../Tools/Log.h"
#include "../Tools/SysIntegration.h"

#include "Utils.h"
#include "Globals.h"

#include "Cell.h"
#include "NeuralNet/DebugTypes.h"

#include <fstream>

#include <stdio.h>

extern void RenderFrame(sf::RenderTexture& texture, uint16_t gen, int frame, bool(*survived)(Cell));
extern void ProduceVideo(uint16_t gen, std::string fps);

extern void ExportNeuralMap(std::shared_ptr<NeuralNet::NeuralMap> map);