#pragma once
#include <cmath>
#include <ostream>
#include <iostream>
#include "vec4.h"
using namespace std;

struct mat4
{
	float elements[16];

	mat4() {
		memset(elements, 0, 16 * sizeof(float));
	}

	mat4(float diagonal) {
		memset(elements, 0, 16 * sizeof(float));
		elements[0] = diagonal;
		elements[5] = diagonal;
		elements[10] = diagonal;
		elements[15] = diagonal;
	}

	mat4& scale_2d(float s) {
		elements[0] /= s;
		elements[5] /= s;
		return *this;
	}

	mat4& mult(const mat4 &other) {
		float data[16];
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				data[col + row * 4] = elements[row * 4] * other.elements[col]
					+ elements[1 + row * 4] * other.elements[col + 4]
					+ elements[2 + row * 4] * other.elements[col + 8]
					+ elements[3 + row * 4] * other.elements[col + 12];
			}
		}
		memcpy(elements, data, 4 * 4 * sizeof(float));
		return *this;
	}

	mat4& rotateZ(float angle) {
		mat4 rot = rotation_matrix(angle, 2);
		return this->mult(rot);
	}

	static mat4 identity_matrix() {
		return mat4(1.f);
	}

	/*
		axis: X = 0, Y = 1, Z = 2
	*/
	static mat4 rotation_matrix(float angle, int axis) {
		float s = sinf(angle), c = cosf(angle);
		mat4 rot(1.0f);
		if (axis == 0) {
			rot.elements[5] = c;
			rot.elements[6] = s;
			rot.elements[9] = -s;
			rot.elements[10] = c;
		}
		else if (axis == 1) {
			rot.elements[0] = c;
			rot.elements[2] = s;
			rot.elements[8] = -s;
			rot.elements[10] = c;
		}
		else {
			rot.elements[0] = c;
			rot.elements[1] = s;
			rot.elements[4] = -s;
			rot.elements[5] = c;
		}
		return rot;
	}

	static mat4 orthographic_matrix(float l, float r, float b, float t, float n, float f) {
		mat4 ortho;
		ortho.elements[0] = 2.f / (r - l);
		ortho.elements[5] = 2.f / (t - b);
		ortho.elements[10] = 2.f / (n - f);

		ortho.elements[3] = (l + r) / (l - r);
		ortho.elements[7] = (b + t) / (b - t);
		ortho.elements[11] = (n + f) / (n - f);
		ortho.elements[15] = 1.f;
		return ortho;
	}

	static mat4 perspective_matrix(float radY, float ratio, float n, float f) {
		mat4 out;
		float a = 1.f / tan(radY / 2.f);

		out.elements[0] = a / ratio;
		out.elements[5] = a;
		out.elements[10] = (n + f) / (n - f);
		out.elements[11] = -1.f;
		out.elements[14] = (2.f * f * n) / (n - f);
		return out;
	}

	static mat4 translation_matrix(const vec3& v) {
		return translation_matrix(v.elements[0], v.elements[1], v.elements[2]);
	}

	static mat4 translation_matrix(float x, float y, float z) {
		mat4 out(1.f);
		out.elements[12] = x;
		out.elements[13] = y;
		out.elements[14] = z;
		return out;
	}
	
	static mat4 look_at(const vec3& eye, const vec3& target, const vec3& up) {
		vec3 v = vec3::normalize(target - eye);
		vec3 r = vec3::normalize(vec3::cross_product(v, up));
		vec3 u = vec3::cross_product(r, v);

		mat4 out;
		out.elements[0] = r[0];
		out.elements[1] = u[0];
		out.elements[2] = -v[0];

		out.elements[4] = r[1];
		out.elements[5] = u[1];
		out.elements[6] = -v[1];

		out.elements[8] = r[2];
		out.elements[9] = u[2];
		out.elements[10] = -v[2];

		out.elements[12] = r * eye;
		out.elements[13] = u * eye;
		out.elements[14] = v * eye;
		out.elements[15] = 1.f;
		return out;
	}

	const float &operator[](int index) const {
		return elements[index];
	}

	float &operator[](int index) {
		return elements[index];
	}

	friend mat4 operator*(mat4 l, const mat4& r) {
		return l.mult(r);
	}

	friend vec4 operator*(const mat4& m4, const vec4& r) {
		const float *l = m4.elements;
		float r0 = l[0] * r[0] + l[1] * r[1] + l[2] * r[2] + l[3] * r[3],
			r1 = l[4] * r[0] + l[5] * r[1] + l[6] * r[2] + l[7] * r[3],
			r2 = l[8] * r[0] + l[9] * r[1] + l[10] * r[2] + l[11] * r[3],
			r3 = l[12] * r[0] + l[13] * r[1] + l[14] * r[2] + l[15] * r[3];
		return vec4(r0, r1, r2, r3);
	}

	friend std::ostream& operator<<(std::ostream& s, const mat4& m) {
		const float *e = m.elements;
		s << "mat4: {" << "\n\t"
			<< e[0] << ", " << e[1] << ", " << e[2] << ", " << e[3] << "\n\t"
			<< e[4] << ", " << e[5] << ", " << e[6] << ", " << e[7] << "\n\t"
			<< e[8] << ", " << e[9] << ", " << e[10] << ", " << e[11] << "\n\t"
			<< e[12] << ", " << e[13] << ", " << e[14] << ", " << e[15] << "\n}";
		return s;
	}
};

