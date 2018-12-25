#pragma once
#include <ostream>

struct vec3 {
	float x, y, z;
	
	vec3() {
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}

	vec3(const float &x, const float &y, const float &z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	static vec3 normalize(const vec3& v) {
		float magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		return vec3(v.x / magnitude, v.y / magnitude, v.z / magnitude);
	}

	static vec3 cross_product(const vec3& l, const vec3& r) {
		return vec3(l.y*r.z - l.z*r.y, l.z*r.x - l.x*r.z, l.x*r.y - l.y*r.x);
	}

	friend float operator*(vec3 left, const vec3& other) {
		return left.x * other.x + left.y * other.y + left.z * other.z;
	}

	friend vec3 operator-(vec3 left, const vec3& other) {
		left.x -= other.x;
		left.y -= other.y;
		left.z -= other.z;
		return left;
	}

	friend vec3 operator+(vec3 left, const vec3& other) {
		left.x += other.x;
		left.y += other.y;
		left.z += other.z;
		return left;
	}

	friend std::ostream& operator<<(std::ostream& s, const vec3& v) {
		s << "vec3: (" << v.x << ", " << v.y << ", " << v.z << ")";
		return s;
	}
};