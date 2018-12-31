#pragma once
#include <cmath>
#include <ostream>
#include <iostream>
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

	mat4& diagonal_scale(float s) {
		elements[0] /= s;
		elements[5] /= s;
		elements[10] /= s;
		elements[15] /= s;
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
		ortho.elements[15] = 1.f;
		ortho.elements[0] = 2.f / (r - l);
		ortho.elements[5] = 2.f / (t - b);
		ortho.elements[10] = 2.f / (n - f);

		ortho.elements[3] = (l + r) / (l - r);
		ortho.elements[7] = (b + t) / (b - t);
		ortho.elements[11] = (n + f) / (n - f);
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

	static mat4 translation_matrix(float x, float y, float z) {
		mat4 out(1.f);
		out.elements[3] = x;
		out.elements[7] = y;
		out.elements[11] = z;
		return out;
	}
	
	static mat4 look_at(const vec3& eye, const vec3& target, const vec3& up) {
		vec3 v = vec3::normalize(target - eye);
		vec3 r = vec3::normalize(vec3::cross_product(v, up));
		vec3 u = vec3::cross_product(r, v);

		mat4 out;
		out.elements[0] = r.x;
		out.elements[1] = u.x;
		out.elements[2] = -v.x;

		out.elements[4] = r.y;
		out.elements[5] = u.y;
		out.elements[6] = -v.y;

		out.elements[8] = r.z;
		out.elements[9] = u.z;
		out.elements[10] = -v.z;

		out.elements[12] = r * eye;
		out.elements[13] = u * eye;
		out.elements[14] = v * eye;
		out.elements[15] = 1.f;
		return out;
	}

	friend mat4 operator*(mat4 l, const mat4& r) {
		return l.mult(r);
	}

	friend std::ostream& operator<<(std::ostream& s, mat4& m) {
		float *e = m.elements;
		s << "mat4: {" << "\n\t"
			<< e[0] << ", " << e[1] << ", " << e[2] << ", " << e[3] << "\n\t"
			<< e[4] << ", " << e[5] << ", " << e[6] << ", " << e[7] << "\n\t"
			<< e[8] << ", " << e[9] << ", " << e[10] << ", " << e[11] << "\n\t"
			<< e[12] << ", " << e[13] << ", " << e[14] << ", " << e[15] << "\n}";
		return s;
	}
};

