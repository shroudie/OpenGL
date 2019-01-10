#pragma once
#include <ostream>

struct vec2 {
	float elements[2];

	vec2() {
		this->elements[0] = 0.f;
		this->elements[1] = 0.f;
	}

	vec2(const float &x, const float &y) {
		this->elements[0] = x;
		this->elements[1] = y;
	}

	static vec2 normalize(const vec2& v) {
		float magnitude = sqrt(v[0] * v[0] + v[1] * v[1]);
		return vec2(v[0] / magnitude, v[1] / magnitude);
	}

	float &operator[](int index) {
		return elements[index];
	}

	friend float operator*(vec2 left, const vec2& other) {
		return left[0] * other[0] + left[1] * other[1];
	}

	friend vec2 operator-(vec2 left, const vec2& other) {
		left[0] -= other[0];
		left[1] -= other[1];
		return left;
	}

	friend vec2 operator+(vec2 left, const vec2& other) {
		left[0] += other[0];
		left[1] += other[1];
		return left;
	}

	friend std::ostream& operator<<(std::ostream& s, const vec2& v) {
		s << "vec2: (" << v[0] << ", " << v[1] << ")";
		return s;
	}
};