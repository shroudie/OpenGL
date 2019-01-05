#pragma once
#include "layer.h"

class Object : public Layer {
public:
	Object(const char*);
	bool is_inside_object(float, float);
};