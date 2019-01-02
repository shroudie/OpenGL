#pragma once

#include "vec3.h"
#include "mat4.h"
#include <vector>

#define PI 3.14159265358979323846

static float degreeToRadius(float degree) {
	return degree * (float)PI / 180;
}

static float triangle_area(float x1, float y1, float x2, float y2, float x3, float y3) {
	return abs(x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.f;
}