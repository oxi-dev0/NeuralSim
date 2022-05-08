#include "../Generation.h"



bool Probability(float factor)
{
	assert(factor >= 0.0 && factor <= 1.0);
	return (Utils::RandRange() / (float)Random::RANDOM_UINT_MAX) < factor;
}


void ProcessEffectorQueue(int owner) {
	std::tuple<float,float> totalMovement;
	for (auto& action : Queues::EffectorQueue[owner]) {
		auto cell = Globals::GlobalGrid.cells[owner];

		if (cell == nullptr) { continue; }


		// MOVEMENT ACTIONS
		// Sum up all wanted movements
		Vector2D rdir = RandomDir();
		float totalX = 0;
		float totalY = 0;

		switch (action->action) {
		case NeuralNet::Effector::MOVE_X:
		{
			Vector2D mov = Vector2D(1, 0);
			if (action->amnt < 0) {
				mov = mov * -1;
			}
			totalX = totalX + (((float)mov.x) * action->amnt);
			totalY = totalY + (((float)mov.y) * action->amnt);
		} break;

		case NeuralNet::Effector::MOVE_Y:
		{
			Vector2D mov = Vector2D(0, 1);
			if (action->amnt < 0) {
				mov = mov * -1;
			}
			totalX = totalX + (((float)mov.x) * action->amnt);
			totalY = totalY + (((float)mov.y) * action->amnt);
		} break;
		 
		case NeuralNet::Effector::MOVE_FB:
		{
			Vector2D mov = cell->dir.asNormalisedVect();
			if (action->amnt < 0) {
				mov = mov * -1;
			}
			totalX = totalX + (((float)mov.x) * action->amnt);
			totalY = totalY + (((float)mov.y) * action->amnt);
		} break;

		case NeuralNet::Effector::MOVE_LR:
		{
			Vector2D mov = cell->dir.rotate(-2).asNormalisedVect();
			if (action->amnt < 0) {
				mov = mov * -1;
			}
			totalX = totalX + (((float)mov.x) * action->amnt);
			totalY = totalY + (((float)mov.y) * action->amnt);
		} break;

		case NeuralNet::Effector::MOVE_RANDOM:
		{
			totalX = totalX + (((float)rdir.x) * action->amnt);
			totalY = totalY + (((float)rdir.y) * action->amnt);
		} break;

		case NeuralNet::Effector::MOVE_NORTH:
		{
			totalY = totalY + (-1 * action->amnt);
		} break;

		case NeuralNet::Effector::MOVE_SOUTH:
		{
			totalY = totalY + (1 * action->amnt);
		} break;

		case NeuralNet::Effector::MOVE_EAST:
		{
			totalX = totalX + (1 * action->amnt);
		} break;

		case NeuralNet::Effector::MOVE_WEST:
		{
			totalX = totalX + (-1 * action->amnt);
		} break;

		}

		std::get<0>(totalMovement) += totalX;
		std::get<1>(totalMovement) += totalY;
	}
	Queues::EffectorQueue[owner].clear();

	// pass total movement through tanh, then use as probability, then multiply by sum component signum to get final movement
	auto cell = Globals::GlobalGrid.cells[owner];

	float sx = std::tanh(std::get<0>(totalMovement));
	float sy = std::tanh(std::get<1>(totalMovement));

	int16_t probX = (int16_t)Probability(std::abs(sx));
	int16_t probY = (int16_t)Probability(std::abs(sy));

	int16_t sigX = sx < 0 ? -1 : 1;
	int16_t sigY = sy < 0 ? -1 : 1;

	Vector2D movementF = Vector2D(probX * sigX, probY * sigY);
	Globals::GlobalGrid.MoveCell(cell->pos, movementF);
}