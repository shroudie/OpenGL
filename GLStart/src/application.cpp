#include "application.h"

GLuint Application::shader_id = -1;
GLFWwindow *Application::window = NULL;
Shader Application::shader = Shader();
deque<Layer*> Application::layers = {};
