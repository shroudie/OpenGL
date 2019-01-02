#pragma once

#include "layer.h"

class Solid : public Layer {
public:
	Solid();
	bool is_inside_object(float, float);
};