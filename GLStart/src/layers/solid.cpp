#include "solid.h"

Solid::Solid() : Layer()
{
	this->set_layer_name("solid");
	data = {
		-0.8f, -0.8f, 0.f, 1.f, 1.f, 1.f,
		0.8f, -0.8f, 0.f, 1.f, 1.f, 1.f,
		0.8f,  0.8f, 0.f, 1.f, 1.f, 1.f,
		0.8f,  0.8f, 0.f, 1.f, 1.f, 1.f,
		-0.8f, 0.8f, 0.f, 1.f, 1.f, 1.f,
		-0.8f, -0.8f, 0.f, 1.f, 1.f, 1.f
	};
	this->numItems = 6;
}

bool Solid::is_inside_object(float x, float y)
{
	return false;
}