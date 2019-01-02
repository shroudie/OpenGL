#include "shape.h"

Shape::Shape(int n)
	: Layer()
{
	angles = n;
	if (n == 0) {

	}
	else if (n == 3) {
		this->set_layer_name("triangle");
		data = {
			-0.6f, -0.4f, 0.f, 1.f, 0.f, 0.f,
			0.6f, -0.4f, 0.f, 0.f, 1.f, 0.f,
			0.f,  0.6f, 0.f, 0.f, 0.f, 1.f
		};
		this->numItems = 3;
		is_inside_object(0.f, -0.4f);
	}
}

bool Shape::is_inside_object(float x, float y)
{
	cout << x << ", " << y << endl;
	if (angles == 3) {
		float x1 = data[0], y1 = data[1], x2 = data[6], y2 = data[7], x3 = data[13], y3 = data[14];
		float A = triangle_area(x1, y1, x2, y2, x3, y3);
		float A1 = triangle_area(x, y, x2, y2, x3, y3);
		float A2 = triangle_area(x1, y1, x, y, x3, y3);
		float A3 = triangle_area(x1, y1, x2, y2, x, y);
		cout << A << " " << A1 << " " << A2 << " " << A3 << endl;
		return A >= (A1 + A2 + A3);
	}
	return false;
}
