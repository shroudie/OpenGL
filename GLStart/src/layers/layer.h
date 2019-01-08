#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../maths/maths.h"

struct VertexArrayObject {
	GLuint vbo[3];
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLuint> indices;
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
		glGenBuffers(3, buffers.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffers.vertices.size(), &buffers.vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffers.normals.size(), &buffers.normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.vbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffers.indices.size() * sizeof(GLuint), &buffers.indices[0], GL_STATIC_DRAW);
	}

	//TODO: is it necessary to unbind on each draw?
	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.vbo[2]);
		glDrawElements(GL_TRIANGLES, buffers.indices.size(), GL_UNSIGNED_INT, 0);
	}

	virtual bool is_inside_object(float, float) = 0;

protected:
	VertexArrayObject buffers;
	char* name;
};