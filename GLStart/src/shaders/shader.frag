#version 330 core

layout (location = 0) out vec4 gl_FragColor;
in vec4 color;

void main () {
	gl_FragColor = color;
}