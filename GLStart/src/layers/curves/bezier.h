#pragma once
#include "curve.h"

class Bezier : public Curve {
public:
	Bezier();
	Bezier(const Point&, const Point&, const Point&, const Point&);
	const Point& lerp(const Point&, const Point&, const Point&, const Point&, float);
};