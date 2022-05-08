#include "Generation.h"

Debug::Timer gtimer;

bool HasSurvived(Cell cell) {
	Vector2D midpoint = Vector2D(Utils::GlobalConfig.sizeX / 2, Utils::GlobalConfig.sizeY / 2);
	return std::sqrt((std::abs(midpoint.x - cell.pos.x) ^ 2) + (std::abs(midpoint.y - cell.pos.y) ^ 2)) < 50;


// TEST REQUIREMENT: BE OVER HALFWAY X
return cell.pos.x > Utils::GlobalConfig.sizeX / 2;
}

void NewGeneration(std::vector<NeuralNet::Genome> survivorsGenomes, bool initGen) {
	for (int i = 0; i < Utils::GlobalConfig.population; i++) {
		bool valid = false;
		while (!valid) {
			Vector2D pos = Vector2D(Utils::RandRange(0, Utils::GlobalConfig.sizeX - 1), Utils::RandRange(0, Utils::GlobalConfig.sizeY - 1));
			auto newCell = std::shared_ptr<Cell>(new Cell(pos));
			if (Globals::GlobalGrid.isEmptyAt(pos)) {
				valid = true;
				Globals::GlobalGrid.NewCell(newCell);
				// map needs cell id
				newCell->InitaliseMap();

				NeuralNet::Genome genome;
				if (initGen) {
					genome = NeuralNet::RandomGenome();
				}
				else {
					genome = NeuralNet::GenerateChild(survivorsGenomes);
				}

				// create neural map
				newCell->InitialiseGenome(genome);
			}
			else {
				uint16_t val = Globals::GlobalGrid.columns[pos.x][pos.y];
			}
		}
	}

	gtimer = Debug::Timer();
	Utils::GlobalSimData.renderGen = Utils::GlobalSimData.currentGen % Utils::GlobalConfig.gensBetweenRender == 0;
}

void EndGeneration() {
	std::vector<NeuralNet::Genome> survivingGenomes;

	if (Utils::GlobalSimData.renderGen) {
		ExportNeuralMap(Globals::GlobalGrid.cells[Utils::RandRange(1, Globals::GlobalGrid.cells.size() - 1)]->map);
	}

	for (auto& cell : Globals::GlobalGrid.cells) {
		if (cell == nullptr) { continue; }

		if (HasSurvived(*cell)) {
			survivingGenomes.push_back(cell->genome);
		}
		else {
			cell = nullptr;
		}
	}
	Globals::GlobalGrid.Clear();
	Globals::GlobalGrid.cells.clear();

	Globals::GlobalGrid.cells.push_back(nullptr);

	Utils::GlobalSimData.currentGen++;
	Utils::GlobalSimData.currentStep = 0;
	LOG_INFO("Simulated Generation {0} in {1}s", Utils::GlobalSimData.currentGen, gtimer.Elapsed());

	if (Utils::GlobalSimData.renderGen) {
		ProduceVideo(Utils::GlobalSimData.currentGen - 1, Utils::GlobalConfig.framerate);
	}

	NewGeneration(survivingGenomes);
}

void SimulationStep(sf::RenderTexture& texture) {
	Utils::GlobalSimData.currentStep++;

	// IMPLEMENT THREADED
	std::vector<std::thread> threads;
	for (auto& cell : Globals::GlobalGrid.cells) {
		if (cell == nullptr) { continue; }
		threads.push_back(std::thread(&Cell::Step, cell));
		 // Step every cell in the system
	}

	for (auto& thread : threads) {
		thread.join();
	}
	threads.clear();

	//for (auto& cell : Globals::GlobalGrid.cells) {
	//	if (cell == nullptr) { continue; }
	//	threads.push_back(std::thread(ProcessEffectorQueue, cell->id));
	//	// Process the queue for every cell
	//}

	for (auto& cell : Globals::GlobalGrid.cells) {
		if (cell == nullptr) { continue; }
		ProcessEffectorQueue(cell->id);
	}

	/*for (auto& thread : threads) {
		thread.join();
	}
	threads.clear();*/

	if (Utils::GlobalSimData.renderGen) {
		RenderFrame(texture, Utils::GlobalSimData.currentGen, Utils::GlobalSimData.currentStep);
	}

	if (Utils::GlobalSimData.currentStep >= Utils::GlobalConfig.stepsPerGen) {

		EndGeneration();
	}
}