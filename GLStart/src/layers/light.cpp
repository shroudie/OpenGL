#include "light.h"

Light::Light(GLint shader_id) : Layer()
{
	this->name = _strdup("light");
	light_position_id = glGetUniformLocation(shader_id, "uLightPosition");
	ambient_color_id = glGetUniformLocation(shader_id, "uAmbientLightColor");
	diffuse_color_id = glGetUniformLocation(shader_id, "uDiffuseLightColor");
	specular_color_id = glGetUniformLocation(shader_id, "uSpecularLightColor");

	kambient_id = glGetUniformLocation(shader_id, "uKAmbient");
	kdiffuse_id = glGetUniformLocation(shader_id, "uKDiffuse");
	kspecular_id = glGetUniformLocation(shader_id, "uKSpecular");
	kshiness_id = glGetUniformLocation(shader_id, "uShininess");

}

void Light::draw() 
{
	//cout << "calling child" << endl;
	glUniform3f(light_position_id, this->position[0], this->position[1], this->position[2]);
	glUniform3f(ambient_color_id, 0.f, 0.f, 0.f);
	glUniform3f(diffuse_color_id, 1.f, 1.f, 1.f);
	glUniform3f(specular_color_id, 1.f, 1.f, 1.f);

	glUniform1f(kshiness_id, 23.f);
	glUniform3f(kambient_id, 1.f, 1.f, 1.f);
	glUniform3f(kdiffuse_id, 205.f / 255.f, 163.f / 255.f, 63.f / 255.f);
	glUniform3f(kspecular_id, 1.f, 1.f, 1.f);
}

bool Light::is_inside_object(float x, float y)
{
	return false;
}