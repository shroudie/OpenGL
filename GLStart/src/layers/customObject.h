#pragma once
#include "layer.h"

class customObject : public Layer {
public:
	customObject(const char*);
	bool is_inside_object(float, float);
	void generate_normals();
};