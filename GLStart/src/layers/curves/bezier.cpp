#include "bezier.h"

Bezier::Bezier() : Curve()
{

}

Bezier::Bezier(const Point &i0, const Point &i1, const Point &i2, const Point &i3) : Curve(i0, i1, i2, i3)
{
	vertexes.push_back(i1.px);
	vertexes.push_back(i1.py);
	vertexes.push_back(-5.f);
	colors.push_back(1.f);
	colors.push_back(0.f);
	colors.push_back(0.f);
	for (unsigned int i = 0; i <= division; ++i) {
		float t = (float)i / division;
		Point p = lerp(i0, i1, i2, i3, t);
		vertexes.push_back(p.px);
		vertexes.push_back(p.py);
		vertexes.push_back(-5.f);
		colors.push_back(1.f);
		colors.push_back(0.f);
		colors.push_back(0.f);
	}
}

const Point& Bezier::lerp(const Point &p0, const Point &p1, const Point &p2, const Point &p3, float t)
{
	float r0 = pow(1 - t, 3),
		r1 = 3 * pow(1 - t, 2) * t,
		r2 = 3 * (1 - t) * pow(t, 2),
		r3 = pow(t, 3);
	float x = r0 * p0.px + r1 * p1.px + r2 * p2.px + r3 * p3.px;
	float y = r0 * p0.py + r1 * p1.py + r2 * p2.py + r3 * p3.py;
	return Point(x, y);
}
