#pragma once

#include "vec3.h"
#include "mat4.h"

#define PI 3.14159265358979323846

static float degreeToRadius(float degree) {
	return degree * (float)PI / 180;
}