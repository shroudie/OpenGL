#pragma once

#include "layer.h"

class Light : public Layer {
public:
	Light(GLint);
	~Light();

	void draw();
	bool is_inside_object(float, float);

	GLint light_position_id, ambient_color_id, diffuse_color_id, specular_color_id;
	GLint kambient_id, kdiffuse_id, kspecular_id, kshiness_id;

	vec3 uLightPosition = vec3(0.f, 5.f, 5.f);
	vec3 uAmbientLightColor = vec3(0.f, 0.f, 0.f);
	vec3 uDiffuseLightColor = vec3(1.f, 1.f, 1.f);
	vec3 uSpecularLightColor = vec3(0.f, 0.f, 0.f);

	vec3 uKAmbient = vec3(1.f, 1.f, 1.f);
	vec3 uKDiffuse = vec3(205 / 255.f, 163 / 255.f, 63 / 255.f);
	vec3 uKSpecular = vec3(0.f, 0.f, 0.f);
	float uShininess = 23.f;
};