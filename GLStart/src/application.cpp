#include "application.h"

GLuint Application::shader_id = -1;
GLFWwindow *Application::window = NULL;
Shader Application::shader = Shader();
Renderer Application::renderer = Renderer();
double Application::cursor_x = 0;
double Application::cursor_y = 0;
bool Application::registered = false;
bool Application::enable_3d = true;
GLint Application::un_matrix_id = -1;