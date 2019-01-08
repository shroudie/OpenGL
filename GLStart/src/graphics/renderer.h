#pragma once
#include <deque>
#include "../layers/solid.h"

struct Camera {
	vec3 position;
	vec3 rotation;
};

/* TODO: IS GLMAP NECESSARY? */
class Renderer {
public:
	Renderer();
	~Renderer();
	void submit(Layer*);
	void flush();
	void move(int, int);
	inline unsigned int count() { return layers.size(); }
	inline const char* layer_name(unsigned int i) { return layers[i]->get_layer_name(); }

private:
	deque<Layer*> layers;
};