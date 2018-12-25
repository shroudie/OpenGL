#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../maths/maths.h"
#include "../utils.h"

class Shader
{
public:
	Shader(const char *, const char *);
	GLuint load_shaders();
	int init_matrices(const char*, const char*, const char*);
	void upload_pr_matrix(const mat4&);
	void upload_vw_matrix(const mat4&);
	void upload_ml_matrix(const mat4&);

	~Shader();

private:
	GLuint shader_id;
	GLint pr_matrix_id, vw_matrix_id, ml_matrix_id;
	const char *vert_path, *frag_path;
};

