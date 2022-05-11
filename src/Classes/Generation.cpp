#include "Generation.h"

Debug::Timer gtimer;

bool HasSurvived(Cell cell) {
	// BE IN CENTER CIRCLE
	/*Vector2D midpoint = Vector2D(Utils::GlobalConfig.sizeX / 2, Utils::GlobalConfig.sizeY / 2);
	return std::sqrt((std::abs(midpoint.x - cell.pos.x) ^ 2) + (std::abs(midpoint.y - cell.pos.y) ^ 2)) < 5;*/

	bool survived = false;
	int c = 0;
	for (auto circle : Survival::SurvivalConfig.survivalCircs) {
		if (Survival::SurvivalConfig.evaluatedCircs[c]) {
			// Shape passes config condition, use to determine survival
			bool cellInside = std::sqrt(pow(std::abs(circle.pos.x - cell.pos.x), 2) + pow(std::abs(circle.pos.y - cell.pos.y), 2)) <= circle.radius;
			survived = survived || cellInside;
		}
		c++;
	}
	int r = 0;
	for (auto rect : Survival::SurvivalConfig.survivalRects) {
		if (Survival::SurvivalConfig.evaluatedRects[r]) {
			// Shape passes config condition, use to determine survival
			bool cellInside = (cell.pos.x >= rect.left) && (cell.pos.x <= rect.right) && (cell.pos.y >= rect.top) && (cell.pos.y <= rect.bottom);
			survived = survived || cellInside;
		}
	}

	return survived;
	// TEST REQUIREMENT: BE OVER HALFWAY X
	// return cell.pos.x > Utils::GlobalConfig.sizeX / 2;
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

	if (Utils::GlobalSimData.renderGen) {
		LOG_INFO("Rendering Generation {0}...", Utils::GlobalSimData.currentGen);
	}
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
	if (Utils::GlobalSimData.renderGen) {
		LOG_INFO("Generation[{0}]: {1}/{2} survived (Rendering took {3}s)", Utils::GlobalSimData.currentGen - 1, survivingGenomes.size(), Utils::GlobalConfig.population, gtimer.Elapsed());
	}
	else {
		LOG_INFO("Generation[{0}]: {1}/{2} survived (Simulation took {3}s)", Utils::GlobalSimData.currentGen - 1, survivingGenomes.size(), Utils::GlobalConfig.population, gtimer.Elapsed());
	}


	if (Utils::GlobalSimData.renderGen) {
		ProduceVideo(Utils::GlobalSimData.currentGen - 1, Utils::GlobalConfig.framerate);
	}

	NewGeneration(survivingGenomes);
}

void SimulationStep(sf::RenderTexture& texture) {
	Utils::GlobalSimData.currentStep++;

	// Evaluate the survival shape conditions
	Survival::SurvivalConfig.EvaluateShapes();

	// Threads could be slowing this down when population rises
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
		RenderFrame(texture, Utils::GlobalSimData.currentGen, Utils::GlobalSimData.currentStep, &HasSurvived);
	}

	if (Utils::GlobalSimData.currentStep >= Utils::GlobalConfig.stepsPerGen) {

		EndGeneration();
	}
}