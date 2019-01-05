#include "application.h"

GLuint Application::shader_id = -1;
GLFWwindow *Application::window = NULL;
Shader Application::shader = Shader();
deque<Layer*> Application::layers = {};
deque<Layer*>::iterator Application::selected = layers.end();
double Application::cursor_x = 0;
double Application::cursor_y = 0;
bool Application::registered = false;
bool Application::enable_3d = true;