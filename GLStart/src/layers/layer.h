#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../maths/maths.h"

struct DataBuffer {
	GLuint vbo;
	vector<GLfloat> datas;
};

struct IndexBuffer {
	GLuint ibo;
	vector<GLushort> indices;
};

struct VertexArrayObject {
	GLuint vbo[2];
	vector<GLfloat> vertices;
	vector<GLushort> indices;
};

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

	void move(float x, float y) {
		/*
		for (int i = 0; i < numItems; ++i) {
			data[6 * i] += x;
			data[6 * i + 1] += y;
		}
		*/
	}

	void load_buffers() {
		glGenBuffers(2, buffers.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffers.vertices.size(), &buffers.vertices[0], GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.vbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffers.indices.size() * sizeof(GLushort), &buffers.indices[0], GL_STATIC_DRAW);
	}

	//TODO: is it necessary to unbind on each draw?
	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));

		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[1]);
		glDrawElements(GL_TRIANGLES, buffers.indices.size(), GL_UNSIGNED_SHORT, 0);
	}

	virtual bool is_inside_object(float, float) = 0;

protected:
	VertexArrayObject buffers;
	char* name;
};