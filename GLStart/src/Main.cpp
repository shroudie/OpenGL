/*#include "graphics/window.h"

int main(void)
{
	Window window("Title", 800, 600);
	while (!window.closed())
	{
		window.update();
	}
	return 0;
}*/


#include "graphics/shader.h"
#include "graphics/window.h"
#include "graphics/camera.h"
#include "graphics/imGuiContainer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "layers/solid.h"

#include "test.h"

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

static const GLint g_index_buffer_data[] = {
   0, 1, 2,
   2, 1, 3,
};

int main(void)
{

	if (!glfwInit())
		::exit(EXIT_FAILURE);
	Window window("Test", 960, 540);
	Shader shader("src/shaders/shader.vert", "src/shaders/shader.frag");
	GLuint program = shader.load_shaders();
	shader.init_matrices("pr_matrix", "vw_matrix", "ml_matrix");

	/*
	GLuint vbo, ibo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_STATIC_DRAW);
*/
	glUseProgram(program);

	mat4 projection = mat4::perspective_matrix(degreeToRadius(45.f), (float)window.getWidth() / (float)window.getHeight(), 0.1f, 200.f);
	mat4 view = mat4::look_at(vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
	mat4 model = mat4::identity_matrix();
	shader.upload_pr_matrix(projection);
	shader.upload_vw_matrix(view);
	shader.upload_ml_matrix(model);
	
	ImGuiContainers::init_window_content(window.get_glfwWindow());
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	vec3 viewDir(0.f, 0.f, -1.f);
	float x = 0.f, y = 0.f, z = 0.f;
	bool fixed = false;

	ImGuiContainers imguis;

	while (!window.closed()) {
		window.clear();

		ImGuiContainers::new_frame();

		//ImGui::ShowDemoWindow();

		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		imguis.setup_menu();

		if (fixed) {
			view = mat4::look_at(vec3(x, y, z), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
		} else {
			vec3 target = vec3(x, y, z) + viewDir;
			view = mat4::look_at(vec3(x, y, z), target, vec3(0.f, 1.f, 0.f));
		}
		shader.upload_vw_matrix(view);


		imguis.render_layers();

		ImGuiContainers::render();

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		window.update();
	}

	ImGuiContainers::destroy();
	::exit(EXIT_SUCCESS);
}
