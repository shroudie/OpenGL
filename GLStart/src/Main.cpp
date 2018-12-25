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


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "graphics/shader.h"
#include "graphics/window.h"
#include "graphics/camera.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define GLSL_VERSION "#version 130"

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
		exit(EXIT_FAILURE);
	Window window("Test", 960, 540);
	Shader shader("src/shaders/shader.vert", "src/shaders/shader.frag");
	GLuint program = shader.load_shaders();
	shader.init_matrices("pr_matrix", "vw_matrix", "ml_matrix");

	GLuint vbo, ibo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

/*
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
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(window.get_glfwWindow(), true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	vec3 viewDir(0.f, 0.f, -1.f);
	bool show_demo_window = true, show_another_window = false;
	float x = 0.f, y = 0.f, z = 0.f;
	bool fixed = false;
	while (!window.closed()) {
		window.clear();

		// ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/*
		ImGui::Begin("Control");
		ImGui::Checkbox("fixed", &fixed);
		ImGui::SliderFloat("pos-x", &x, 0.0f, 100.0f); // slider from 0.0 to 100.0
		ImGui::SliderFloat("pos-y", &y, 0.0f, 100.0f); 
		ImGui::SliderFloat("pos-z", &z, 0.0f, 100.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		ImGui::SameLine();
		ImGui::End();
		*/
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
			//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		if (fixed) {
			view = mat4::look_at(vec3(x, y, z), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
		} else {
			vec3 target = vec3(x, y, z) + viewDir;
			view = mat4::look_at(vec3(x, y, z), target, vec3(0.f, 1.f, 0.f));
		}
		shader.upload_vw_matrix(view);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ImGui render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		window.update();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	exit(EXIT_SUCCESS);
}
