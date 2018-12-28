#pragma once

#include "layer.h"

class Solid : public Layer {
public:
	Solid(int);
	Solid(vector<GLfloat> &, vector<GLfloat> &);
	~Solid();
	void bind();

private:
	int shape;
	vector<GLfloat> vertex, color; //vexter position or color position;
};