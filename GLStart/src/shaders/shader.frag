#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
precision mediump float;

layout (location = 0) out vec4 gl_FragColor;

uniform vec3 uLightPosition;
uniform vec3 uAmbientLightColor;
uniform vec3 uDiffuseLightColor;
uniform vec3 uSpecularLightColor;

uniform vec3 uKAmbient;
uniform vec3 uKDiffuse;
uniform vec3 uKSpecular;
uniform float uShininess;

uniform mat3 un_matrix;

uniform samplerCube skybox;
uniform bool use_texture;
uniform bool use_light;


in vec3 position;
in vec3 normal;

void main () {
	if (use_texture) {
		gl_FragColor = textureCube(skybox, normalize(position));
	}
	if (use_light) {
		vec3 positionEye = position;
		vec3 L = normalize(uLightPosition - positionEye);
		vec3 N = normalize(un_matrix * normal);
		vec3 R = normalize(reflect(-L, N));

		float diffusalWeighting = max(dot(N, L), 0.0);
		vec3 viewEye = -normalize(positionEye);
		float rdotv = max(dot(R, viewEye), 0.0);
		float specularWeighting = pow(rdotv, uShininess);

		gl_FragColor += vec4(((uAmbientLightColor*uKAmbient)
							+ (uDiffuseLightColor*uKDiffuse) * diffusalWeighting
							+ (uSpecularLightColor*uKSpecular) * specularWeighting),1.0);

	}
	else {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
}