#include "../Generation.h"

void ProcessEffectorQueue() {
	for (Queues::ActionEvent action : Queues::EffectorQueue) {
		int owner = action.owner;
		Cell* cell = Globals::GlobalGrid.cells[owner];

		Vector2D offsetX = Vector2D(1, 0);
		Vector2D offsetY = Vector2D(0, 1);

		switch (action.action) {
		case NeuralNet::Effector::MOVE_X:
			if (action.amnt < 0) {
				offsetX = offsetX * -1;
			}
			Globals::GlobalGrid.MoveCell(cell->pos, offsetX);
			break;
		case NeuralNet::Effector::MOVE_Y:
			if (action.amnt < 0) {
				offsetY = offsetY * -1;
			}
			Globals::GlobalGrid.MoveCell(cell->pos, offsetY);
			break;
		default:
			break;
		}
	}
}