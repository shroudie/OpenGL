#pragma once
#include "layer.h"

class Shape : public Layer {
public:
	Shape(int);
	bool is_inside_object(float x, float y);

private:
	int angles = 0;
};