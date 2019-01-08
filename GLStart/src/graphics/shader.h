#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../maths/maths.h"
#include "../utils.h"

struct light_info {
	vec3 uLightPosition = vec3(0.f, 5.f, 5.f);
	vec3 uAmbientLightColor = vec3(0.f, 0.f, 0.f);
	vec3 uDiffuseLightColor = vec3(1.f, 1.f, 1.f);
	vec3 uSpecularLightColor = vec3(0.f, 0.f, 0.f);

	vec3 uKAmbient = vec3(1.f, 1.f, 1.f);
	vec3 uKDiffuse = vec3(205 / 255.f, 163 / 255.f, 63 / 255.f);
	vec3 uKSpecular = vec3(0.f, 0.f, 0.f);
	float uShininess = 23.f;

	GLint light_position_id, ambient_color_id, diffuse_color_id, specular_color_id, kambient_id, kdiffuse_id, kspecular_id, kshiness_id;
};

class Shader
{
public:
	GLint pr_matrix_id, vw_matrix_id, ml_matrix_id;

	Shader() {}
	GLuint load_shaders(const char *, const char *);
	void init_matrix(GLint *, const char*);
	void upload_matrix(const mat4&, GLint);

	void init_light_locations();
	void upload_light_components();
	~Shader();

private:
	GLuint shader_id;
	light_info shader_light_info;
};

