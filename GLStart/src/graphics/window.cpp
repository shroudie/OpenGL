#include "window.h"
#include <Windows.h>

Window::Window() :m_Title("Test")
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	init_window(monitor, mode->width, mode->height);
}

Window::Window(const char * title, int w, int h) :m_Title(title)
{
	init_window(NULL, w, h);
}

Window::~Window()
{
	glfwTerminate();
	glfwDestroyWindow(m_Window);
}

static void exit_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void Window::init_window(GLFWmonitor* monitor, int width, int height)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		std::exit(-1);

	m_Window = glfwCreateWindow(width, height, m_Title, NULL, NULL);
	if (!m_Window)
	{
		glfwTerminate();
		std::exit(-1);
	}
	glfwSetKeyCallback(m_Window, exit_key_callback);
	glfwMakeContextCurrent(m_Window);
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

int Window::getHeight() const
{
	int height;
	glfwGetWindowSize(m_Window, NULL, &height);
	return height;
}

int Window::getWidth() const
{
	int width;
	glfwGetWindowSize(m_Window, &width, NULL);
	return width;
}
