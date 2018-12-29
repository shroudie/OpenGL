#include "imGuiContainer.h"

ImGuiContainers::~ImGuiContainers()
{
	for (int i = 0; i < solids.size(); i++) {
		delete solids[i];
	}
}
