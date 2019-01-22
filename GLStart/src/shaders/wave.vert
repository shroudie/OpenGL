#version 150 core

in vec2 vertexPostion;

void main() {
    gl_Position = vec4(vertexPostion, 0.0, 1.0);
}
