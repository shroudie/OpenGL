#include "solid.h"

Solid::Solid(const Solid& s) : Layer()
{
	this->shape = s.shape;
	this->vertex = vector<GLfloat>(s.vertex);
	this->color = vector<GLfloat>(s.vertex);
}

Solid::Solid(int shape) : Layer()
{
	this->shape = shape;
}

Solid::Solid(vector<GLfloat> &v, vector<GLfloat> &c) : Layer()
{
	vertex = v;
	color = c;
}

Solid::~Solid()
{

}

void Solid::bind()
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex.size(), &vertex[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint cbo;
	glGenBuffers(1, &cbo);
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * color.size(), &color[0], GL_STATIC_DRAW); 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Solid::draw()
{
	this->bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}