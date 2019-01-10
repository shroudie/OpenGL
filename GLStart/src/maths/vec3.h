#pragma once
#include <ostream>

struct vec3 {
	float elements[3];
	
	vec3() {
		this->elements[0] = 0.0f;
		this->elements[1] = 0.0f;
		this->elements[2] = 0.0f;
	}

	vec3(const float &x, const float &y, const float &z) {
		this->elements[0] = x;
		this->elements[1] = y;
		this->elements[2] = z;
	}

	const float &operator[](int index) const {
		return elements[index];
	}

	float &operator[](int index) {
		return elements[index];
	}

	static vec3 normalize(const vec3& v) {
		float magnitude = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		if (magnitude > 0.f) magnitude = 1 / magnitude;
		return vec3(v[0] * magnitude, v[1] * magnitude, v[2] * magnitude);
	}

	static vec3 cross_product(const vec3& l, const vec3& r) {
		float ax = l[0], ay = l[1], az = l[2];
		float bx = r[0], by = r[1], bz = r[2];
		return vec3(ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx);
	}

	friend float operator*(vec3 left, const vec3& other) {
		return left[0] * other[0] + left[1] * other[1] + left[2] * other[2];
	}

	friend vec3 operator/(vec3 left, float scale) {
		left.elements[0] /= scale;
		left.elements[1] /= scale;
		left.elements[2] /= scale;
		return left;
	}

	friend vec3 operator*(vec3 left, float scale) {
		left.elements[0] *= scale;
		left.elements[1] *= scale;
		left.elements[2] *= scale;
		return left;
	}

	friend vec3 operator-(vec3 left, const vec3& other) {
		left[0] -= other[0];
		left[1] -= other[1];
		left[2] -= other[2];
		return left;
	}

	friend vec3 operator+(vec3 left, const vec3& other) {
		left[0] += other[0];
		left[1] += other[1];
		left[2] += other[2];
		return left;
	}

	friend std::ostream& operator<<(std::ostream& s, const vec3& v) {
		s << "vec3: (" << v[0] << ", " << v[1] << ", " << v[2] << ")";
		return s;
	}
};