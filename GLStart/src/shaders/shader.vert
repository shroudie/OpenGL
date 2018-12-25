#version 330 core

layout(location = 0) in vec3 vertexPostion;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix;
uniform mat4 ml_matrix;

void main() {
	gl_Position = pr_matrix * vw_matrix * ml_matrix * vec4(vertexPostion, 1.0);
}
