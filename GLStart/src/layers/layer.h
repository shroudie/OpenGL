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
	Layer() {}
	void set_layer_name(const char *);

protected:
	vec3 position, anchor;
	float opacity;
	string name;
};