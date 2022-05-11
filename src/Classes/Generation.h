#pragma once

#include "Queues.h"

#include "Genome.h"
#include "Output.h"
#include "../Shapes/Shapes.h"

extern Debug::Timer gtimer;

extern inline void ProcessEffectorQueue(int owner);

extern bool HasSurvived(Cell cell);

extern void NewGeneration(std::vector<NeuralNet::Genome> survivorsGenomes, bool initGen = false);

extern void EndGeneration();

extern void SimulationStep(sf::RenderTexture& texture);