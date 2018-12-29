#include "layer.h"

Layer::Layer()
{
	opacity = 100;
}

Layer::Layer(const char* c) {
	name = _strdup(c);
}

Layer::~Layer()
{
	free(name);
}

void Layer::set_layer_name(const char *n)
{
	name = _strdup(n);
}

