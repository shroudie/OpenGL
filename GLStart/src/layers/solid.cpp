#include "solid.h"

Solid::Solid() : Layer()
{
	this->set_layer_name("solid");
	buffers.vertices = {
		-0.8f, -0.8f, -5.f,
		0.8f, -0.8f, -5.f,
		0.8f,  0.8f, -5.f,
		-0.8f, 0.8f, -5.f
	};
	buffers.normals = {
		1.f, 1.f, 1.f,
		1.f, 1.f, 1.f,
		1.f, 1.f, 1.f,
		1.f, 1.f, 1.f
	};
	buffers.indices = {
		0, 1, 2, 
		2, 3, 0
	};
	this->load_buffers();
}

bool Solid::is_inside_object(float x, float y)
{
	/*
	float vertx[] = { -.3f, .3f, 0.f };
	float verty[] = { -.3f, -.3f, 0.6f };
	bool inside = false;
	for (int i = 0, j = numItems - 1; i < numItems; j = i++) {
		int xi = 6 * i, xj = 6 * j, yi = xi + 1, yj = xj + 1;
		if ((data[yi] > y) != (data[yj] > y) &&
			(x < (data[xj] - data[xi]) * (y - data[yi]) / (data[yj] - data[yi]) + data[xi])) {
			inside = !inside;
		}
	}*/
	return false;
}