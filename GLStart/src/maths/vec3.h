#pragma once
#include <ostream>

struct vec3 {
	float elements[3];
	
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
		if (magnitude > 0.f) magnitude = 1 / magnitude;
		return vec3(v.x * magnitude, v.y * magnitude, v.z * magnitude);
	}

	static vec3 cross_product(const vec3& l, const vec3& r) {
		float ax = l.x, ay = l.y, az = l.z;
		float bx = r.x, by = r.y, bz = r.z;
		return vec3(ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx);
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