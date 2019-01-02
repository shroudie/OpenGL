#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
	Window();
	Window(const char *title, int width, int height);
	~Window();
	void init_window(GLFWmonitor* monitor = NULL, int width = 640, int height = 360);
	inline GLFWwindow *get_glfwWindow() { return m_Window; }
	bool closed() const;
	inline void clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	void update() const;
	int getHeight() const;
	int getWidth() const;

private:
	const char *m_Title;
	GLFWwindow* m_Window;
};