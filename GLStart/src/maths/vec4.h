#pragma once
#include <ostream>

struct vec4 {
	float x, y, z, w;

	vec4() {
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
		this->w = 0.0f;
	}

	vec4(const float &x, const float &y, const float &z, const float &w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	friend std::ostream& operator<<(std::ostream& s, const vec4& v) {
		s << "vec4: (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return s;
	}
};