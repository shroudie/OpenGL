#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
precision mediump float;
layout (location = 0) out vec4 gl_FragColor;

uniform samplerCube skybox;
uniform bool apply_texture;

in vec4 color;
in vec3 position;

void main () {
//	if (apply_texture) {
//		gl_FragColor = textureCube(skybox, normalize(position));
//	}
//	else {
		gl_FragColor = color;
//	}
}