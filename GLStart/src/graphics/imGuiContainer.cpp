#include "imGuiContainer.h"

ImGuiContainers::~ImGuiContainers()
{
	for (int i = 0; i < solids.size(); i++) {
		delete solids[i];
	}
	for (int i = 0; i < curves.size(); i++) {
		delete curves[i];
	}
}
