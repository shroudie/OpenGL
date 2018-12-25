#include "window.h"

Window::Window(const char * title, int width, int height)
{
	m_Title = title;
	m_Hieght = height;
	m_Width = width;
	init_window();
}

Window::~Window()
{
	glfwTerminate();
	glfwDestroyWindow(m_Window);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

GLFWwindow * Window::init_window()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		std::exit(-1);

	m_Window = glfwCreateWindow(m_Width, m_Hieght, m_Title, NULL, NULL);
	if (!m_Window)
	{
		glfwTerminate();
		std::exit(-1);
	}
	glfwSetKeyCallback(m_Window, key_callback);
	glfwMakeContextCurrent(m_Window);
	return m_Window;
}

bool Window::closed() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::update() const
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}
