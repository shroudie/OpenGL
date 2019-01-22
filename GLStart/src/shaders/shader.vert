#version 330 core

layout(location = 0) in vec3 vertexPostion;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix;

out vec3 position;
out vec3 normal;

void main() {
	position = vertexPostion;
	normal = vertexNormal;

    gl_Position = pr_matrix*vw_matrix*vec4(vertexPostion, 1.0);
}
