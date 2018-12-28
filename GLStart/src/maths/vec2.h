#pragma once
#include <ostream>

struct vec2 {
	float x, y;

	vec2() {
		this->x = 0.0f;
		this->y = 0.0f;
	}

	vec2(const float &x, const float &y) {
		this->x = x;
		this->y = y;
	}

	static vec2 normalize(const vec2& v) {
		float magnitude = sqrt(v.x * v.x + v.y * v.y);
		return vec2(v.x / magnitude, v.y / magnitude);
	}

	friend float operator*(vec2 left, const vec2& other) {
		return left.x * other.x + left.y * other.y;
	}

	friend vec2 operator-(vec2 left, const vec2& other) {
		left.x -= other.x;
		left.y -= other.y;
		return left;
	}

	friend vec2 operator+(vec2 left, const vec2& other) {
		left.x += other.x;
		left.y += other.y;
		return left;
	}

	friend std::ostream& operator<<(std::ostream& s, const vec2& v) {
		s << "vec2: (" << v.x << ", " << v.y << ")";
		return s;
	}
};