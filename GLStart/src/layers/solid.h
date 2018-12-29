#pragma once

#include "layer.h"

class Solid : public Layer {
public:
	Solid(const Solid&);
	Solid(int);
	Solid(vector<GLfloat> &, vector<GLfloat> &);
	~Solid();
	void bind();
	void draw();
	inline vector<GLfloat>& get_vertexes() { return vertex; }

private:
	int shape;
	vector<GLfloat> vertex, color; //vexter position or color position;
};