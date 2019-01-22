#pragma once
#include <deque>
#include "../layers/solid.h"
#include "../graphics/shader.h"

struct Camera {
	vec3 position;
	vec3 rotation;
	vec3 direction;

	Camera() {
		position = vec3(0.f, 0.f, 1.f);
		rotation = vec3(0.f, 0.f, 0.f);
		direction = vec3(0.f, 0.f, -1.f);
	}

	Camera(const vec3& p, const vec3& r, const vec3& dir) {
		position = p;
		rotation = r;
		direction = dir;
	}

	void move(float x, float y, float z) {
		position.elements[0] = x;
		position.elements[1] = y;
		position.elements[2] = z;
	}
};

struct ShaderMatrix {
	GLint matrix_id;
	void *matrix;
	void(*myf) (GLint location, GLsizei count, GLboolean transpose, void *matrix);
	void(*upload_matrix_ptr) (GLint location, GLsizei count, GLboolean transpose, const float*);

	ShaderMatrix(GLuint shader_id, const char* matrix) {
		matrix_id = glGetUniformLocation(shader_id, matrix);
	}

	void upload_matrix() {
		myf(matrix_id, 1, GL_FALSE, matrix);
	}

	void upload_matrix(const float *value) {
		upload_matrix_ptr(matrix_id, 1, GL_FALSE, value);
	}
};


/* TODO: IS GLMAP NECESSARY? */
class Renderer {
public:
	Renderer();
	~Renderer();
	void init_shader(const char*, const char*);
	void init_camera(vec3 pos);
	void submit(Layer*);
	void flush();
	void set_accept_light();
	void move(int, int);
	void register_camera(const Camera& c);
	void init_uniform_locations();
	inline const char* layer_name(unsigned int i) { return layers[i]->get_layer_name(); }
	inline const vec3& camera_position() { return camera.position; }
	inline const vec3& target_position() { return camera.position + camera.direction; }
	inline void renderer_start() { animated = true; }

//private:
	GLuint shader_id;
	deque<Layer*> layers;
	vector<ShaderMatrix> matrices;

	Camera camera;
	Shader shader;

	GLint use_light_loc, use_texture_loc;

	int frame_count = 30, current_frame = 0;
	float ms_per_frame = 15 / (float)frame_count;
	bool animated = false;
};