#pragma once
#include <glad/glad.h>
#include <vector>
#include <iostream>

struct Point {
	Point() {
		px = (py = 0.f);
	}

	Point(float x, float y) {
		px = x;
		py = y;
	}
	
	Point(const Point& p) {
		this->px = p.px;
		this->py = p.py;
	}

	float px, py;
};

class Curve
{
public:
	Curve();
	Curve(const Point&, const Point&, const Point&, const Point&);
	~Curve() {} 
	inline void set_division_number(int s) { division = s; }
	inline void set_line_width(float w) { line_width = w; }
	void draw_curve();

protected:
	unsigned int division; // number of points between
	Point p0, p1, p2, p3;
	std::vector<GLfloat> vertexes, colors;
	float line_width;
};

