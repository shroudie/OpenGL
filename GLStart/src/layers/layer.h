#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../maths/maths.h"

class Layer {
public:
	Layer() {}

	Layer(const char* s) {
		name = _strdup(s);
	}

	~Layer() {
		free(name);
	}

	void set_layer_name(const char *s) {
		free(name);
		name = _strdup(s);
	}

	const char* get_layer_name() { 
		return name; 
	}

	void bind() {
		GLuint vao;
		glGenBuffers(1, &vao);
		glBindBuffer(GL_ARRAY_BUFFER, vao);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), &data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	}

	void draw() {
		this->bind();
		glDrawArrays(GL_TRIANGLES, 0, numItems);
	}

	virtual bool is_inside_object(float, float) = 0;

protected:
	int numItems = 0;
	float opacity = 100.f;
	vector<GLfloat> data;
	char* name;
};