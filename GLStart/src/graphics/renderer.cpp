#include "renderer.h"

Renderer::Renderer()
{

}

void Renderer::submit(Layer* l)
{
	layers.push_back(l);
}

void Renderer::flush()
{
	for (deque<Layer*>::iterator it = layers.begin(); it != layers.end(); ++it) {
		(*it)->draw();
	}
}

void Renderer::move(int move_from, int move_to)
{
	deque<Layer*>::iterator it = layers.begin() + move_from;
	Layer *cur = *it;
	layers.erase(it);
	it = layers.begin() + move_to;
	layers.insert(it, cur);
}

Renderer::~Renderer()
{
	for (unsigned int i = 0; i < layers.size(); i++) {
		delete layers[i];
	}
}