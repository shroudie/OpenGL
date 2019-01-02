#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../maths/maths.h"
#include "../utils.h"

class Shader
{
public:
	GLint pr_matrix_id, vw_matrix_id, ml_matrix_id;

	Shader() {}
	GLuint load_shaders(const char *, const char *);
	void init_matrix(GLint *, const char*);
	void upload_matrix(const mat4&, GLint);
	~Shader();

private:
	GLuint shader_id;
};

