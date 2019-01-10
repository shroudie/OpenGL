#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utility>
#include "../maths/maths.h"

struct VertexArrayObject {
	GLuint vbo[3];
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLuint> indices;
};

class Layer {
public:
	Layer() {
		position = vec3(0.f, 0.f, 0.f);
		rotation = vec3(0.f, 0.f, 0.f);
	}

	Layer(const char* s) {
		position = vec3(0.f, 0.f, 0.f);
		rotation = vec3(0.f, 0.f, 0.f);
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
	struct key_frame_compare {
		bool operator()(const pair<int, vec3>& left, const pair<int, vec3>& right) {
			return left.first < right.first;
		}
	};

	void add_position_key_frame(int frame, const vec3& pos) {
		pair<int, vec3> new_key_frame = make_pair(frame, pos);
		if (key_position.empty()) {
			key_position.push_back(new_key_frame);
		}
		else {
			vector<pair<int, vec3>>::iterator lower = lower_bound(key_position.begin(), key_position.end(), new_key_frame, key_frame_compare());
			if ((*lower).first == frame) {
				(*lower).second = pos;
			}
			else {
				key_position.insert(lower, new_key_frame);
			}
		}
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

//protected:
	VertexArrayObject buffers;
	vec3 position, rotation;
	vector<pair<int, vec3>> key_position, key_rotation;
	char* name;
};