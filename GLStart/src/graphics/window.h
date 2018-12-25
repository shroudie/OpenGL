#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
	Window(const char *title, int width, int height);
	~Window();
	GLFWwindow *init_window();
	inline GLFWwindow *get_glfwWindow() { return m_Window; }
	bool closed() const;
	inline void clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	void update() const;
	inline int getHeight() const { return m_Hieght; }
	inline int getWidth() const { return m_Width; }

private:
	int m_Width, m_Hieght;
	const char *m_Title;
	GLFWwindow* m_Window;
};