#include "curve.h"

Curve::Curve()
{
	division = 20;
	line_width = 2.f;
}

Curve::Curve(const Point& i0, const Point& i1, const Point& i2, const Point& i3)
	:p0(i0), p1(i1), p2(i2), p3(i3), division(20), line_width(2.f)
{

}

void Curve::draw_curve()
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexes.size(), &vertexes[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint cbo;
	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), &colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glLineWidth(line_width);
	glDrawArrays(GL_LINE_STRIP, 0, division + 1);
}
