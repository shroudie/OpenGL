#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../maths/maths.h"
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Layer {
public:
	Layer();
	Layer(const char*);
	~Layer();
	void set_layer_name(const char *);
	inline const char* get_layer_name() { return name; }

protected:
	vec3 position, anchor;
	float opacity;
	char* name;
};