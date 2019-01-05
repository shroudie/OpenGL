#include "shape.h"

Shape::Shape(int n)
	: Layer()
{
	angles = n;
	if (n == 0) {

	}
	else if (n == 3) {
		this->set_layer_name("triangle");
		buffers.vertices = {
			-0.3f, -0.3f, -5.f, 1.f, 0.f, 0.f,
			0.3f, -0.3f, -5.f, 0.f, 1.f, 0.f,
			0.f,  0.6f, -5.f, 0.f, 0.f, 1.f
		};
		buffers.indices = {
			0, 1, 2
		};
	}
	this->load_buffers();
}

/*
	No idea how it worked, probably intersection counting, REVIEW LATER
*/
bool Shape::is_inside_object(float x, float y)
{
	return false;
}
