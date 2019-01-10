#include "application.h"

GLFWwindow *Application::window;
Renderer Application::renderer;

double Application::cursor_x = 0;
double Application::cursor_y = 0;
bool Application::registered = false;
