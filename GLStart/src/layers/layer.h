#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <png.h>

#include <utility>
#include "../maths/maths.h"

struct VertexArrayObject {
	GLuint vbo[3];
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLuint> indices;
};

struct TextureObject {
	GLuint texture_id;
	GLint skybox_location;
	GLenum texture_target;
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
		delete textures;
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
	virtual void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		if (textures) {
			glBindTexture(textures->texture_target, textures->texture_id);
			//cout << "binding texture: " << textures->texture_id << endl;
			//gl.uniform1i(gl.getUniformLocation(shaderProgram, "uCubeSampler"), 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.vbo[2]);
		glDrawElements(GL_TRIANGLES, buffers.indices.size(), GL_UNSIGNED_INT, 0);
	}

	//void init_uniform_locations(GLuint shader_id) {
	//	apply_texture = glGetUniformLocation(shader_id, "use_texture");
	//	apply_light = glGetUniformLocation(shader_id, "use_light");
	//}

	bool add_texture(const char *fp, GLuint shader_id) {
		FILE *fs = fopen(fp, "rb");
		if (!fs) {
			std::cout << "Unable to open file" << std::endl;
			return false;
		} 

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png_ptr) {
			fclose(fs);
			return false;
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			fclose(fs);
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return false;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(fs);
			/* If we get here, we had a
			 * problem reading the file */
			return false;
		}

		png_init_io(png_ptr, fs);
		png_set_sig_bytes(png_ptr, 0);
		png_read_info(png_ptr, info_ptr);

		//int bit_depth, color_type;
		int width = png_get_image_width(png_ptr, info_ptr);
		int  height = png_get_image_height(png_ptr, info_ptr);
		png_byte color_type = png_get_color_type(png_ptr, info_ptr);
		png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

		if (bit_depth == 16)
			png_set_strip_16(png_ptr);

		if (color_type == PNG_COLOR_TYPE_PALETTE) 
			png_set_palette_to_rgb(png_ptr);

		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
			png_set_expand_gray_1_2_4_to_8(png_ptr);

		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png_ptr);

		if (color_type == PNG_COLOR_TYPE_GRAY)
			png_set_gray_to_rgb(png_ptr);

		png_read_update_info(png_ptr, info_ptr);

		int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
		png_byte *image_data = new png_byte[rowbytes * height];
		png_bytep *row_pointers = new png_bytep[height];

		for (int i = 0; i < height; ++i)
			row_pointers[height - 1 - i] = image_data + i * rowbytes;
		png_read_image(png_ptr, row_pointers);

		textures = new TextureObject();

		GLuint texture_id;
		glGenTextures(1, &texture_id);
		if (false) {
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
			//glBindTexture(GL_TEXTURE_2D, 0);
			textures->texture_target = GL_TEXTURE_2D;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else {
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
			for (unsigned int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
			}
			textures->texture_target = GL_TEXTURE_CUBE_MAP;

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete[] image_data;
		delete[] row_pointers;

		fclose(fs);

		textures->texture_id = texture_id;
		textures->skybox_location = glGetUniformLocation(shader_id, "skybox");
		return true;
	}

	mat4 get_transformation_matrix() {
		return mat4::translation_matrix(position);
	}

	void set_accept_light(bool b) {
		accept_light = b;
	}

	virtual bool is_inside_object(float, float) = 0;

//protected:
	VertexArrayObject buffers;

	TextureObject *textures;

	bool accept_light = false;

	vec3 position, rotation;
	vector<pair<int, vec3>> key_position, key_rotation;

	char* name;
};