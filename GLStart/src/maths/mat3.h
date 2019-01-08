#pragma once
#include <cmath>
#include <ostream>
#include <iostream>
#include "mat4.h"
using namespace std;

struct mat3
{
	float elements[9];

	mat3() {
		memset(elements, 0, 9 * sizeof(float));
	}

	mat3(float diagonal) {
		memset(elements, 0, 9 * sizeof(float));
		elements[0] = diagonal;
		elements[4] = diagonal;
		elements[8] = diagonal;
	}

	mat3& transpose() {
		this->elements[1] = (this->elements[3] = this->elements[1]);
		this->elements[2] = (this->elements[6] = this->elements[2]);
		this->elements[5] = (this->elements[7] = this->elements[5]);
		return *this;
	}

	mat3& invert() {
		float a00 = this->elements[0], a01 = this->elements[1], a02 = this->elements[2];
		float a10 = this->elements[3], a11 = this->elements[4], a12 = this->elements[5];
		float a20 = this->elements[6], a21 = this->elements[7], a22 = this->elements[8];
		float b01 = a22 * a11 - a12 * a21;
		float b11 = -a22 * a10 + a12 * a20;
		float b21 = a21 * a10 - a11 * a20;
		// Calculate the determinant
		float det = a00 * b01 + a01 * b11 + a02 * b21;
		if (!det) {
			return *this;
		}
		det = 1.f / det;
		this->elements[0] = b01 * det;
		this->elements[1] = (-a22 * a01 + a02 * a21) * det;
		this->elements[2] = (a12 * a01 - a02 * a11) * det;
		this->elements[3] = b11 * det;
		this->elements[4] = (a22 * a00 - a02 * a20) * det;
		this->elements[5] = (-a12 * a00 + a02 * a10) * det;
		this->elements[6] = b21 * det;
		this->elements[7] = (-a21 * a00 + a01 * a20) * det;
		this->elements[8] = (a11 * a00 - a01 * a10) * det;
		return *this;
	}

	static mat3 from_mat4(const mat4& m) {
		const float *e = m.elements;
		mat3 out;
		
		out.elements[0] = e[0];
		out.elements[1] = e[1];
		out.elements[2] = e[2];
		out.elements[3] = e[4];
		out.elements[4] = e[5];
		out.elements[5] = e[6];
		out.elements[6] = e[8];
		out.elements[7] = e[9];
		out.elements[8] = e[10];
		return out;
	}

	friend std::ostream& operator<<(std::ostream& s, mat3& m) {
		float *e = m.elements;
		s << "mat3: {" << "\n\t"
			<< e[0] << ", " << e[1] << ", " << e[2] << "\n\t"
			<< e[3] << ", " << e[4] << ", " << e[5] << "\n\t"
			<< e[6] << ", " << e[7] << ", " << e[8] << "\n\t";
		return s;
	}
};

