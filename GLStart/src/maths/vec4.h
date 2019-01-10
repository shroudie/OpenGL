#pragma once
#include <ostream>

struct vec4 {
	float elements[4];

	vec4() {
		memset(elements, 0, 4 * sizeof(float));
	}

	vec4(const float &x, const float &y, const float &z, const float &w) {
		this->elements[0] = x;
		this->elements[1] = y;
		this->elements[2] = z;
		this->elements[3] = w;
	}

	const float &operator[](int index) const {
		return elements[index];
	}

	float &operator[](int index) {
		return elements[index];
	}

	friend std::ostream& operator<<(std::ostream& s, const vec4& v) {
		s << "vec4: (" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
		return s;
	}
};