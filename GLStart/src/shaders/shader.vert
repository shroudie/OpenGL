#version 330 core

layout(location = 0) in vec3 vertexPostion;
layout(location = 1) in vec3 vertexColor;
out vec4 color;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix;
uniform mat4 ml_matrix;

void main() {
	color = vec4(vertexColor, 1.0);
	gl_Position = pr_matrix * vw_matrix * ml_matrix * vec4(vertexPostion, 1.0);
}
