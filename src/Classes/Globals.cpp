#include "Globals.h"

namespace Globals {
	Grid GlobalGrid;
	void Init() {
		GlobalGrid = Grid();
		GlobalGrid.Init(Utils::GlobalConfig.sizeX, Utils::GlobalConfig.sizeY);
	}
}