#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "graphics/shader.h"
#include "graphics/window.h"
#include "layers/solid.h"
#include "layers/shape.h"
#include "layers/curves/bezier.h"

#include <deque>

#define GLSL_VERSION "#version 130"

class Application {
public:
	static GLuint shader_id;
	static GLFWwindow *window;
	static Shader shader; 

	static deque<Layer*> layers;

	static bool should_close() {
		return glfwWindowShouldClose(window);
	}

	static void clear_window() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void update_window() {
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	static void init_window_components() {
		if (!glfwInit())
			::exit(EXIT_FAILURE);
		glfwSetErrorCallback(error_callback);
		window = glfwCreateWindow(1280, 720, "test", NULL, NULL);
		if (!window) {
			glfwTerminate();
			::exit(-1);
		}
		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, exit_key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
	}

	static void init_shader_components() {
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		shader_id = shader.load_shaders("src/shaders/shader.vert", "src/shaders/shader.frag");
		shader.init_matrix(&shader.pr_matrix_id, "pr_matrix");
		shader.init_matrix(&shader.ml_matrix_id, "ml_matrix");
		glUseProgram(shader_id);
	}

	static void upload_matrices() {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		float ratio = width / (float)height;
		glViewport(0, 0, width, height);

		mat4 ml = mat4::identity_matrix();
		mat4 pr = mat4::orthographic_matrix(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		shader.upload_matrix(ml, shader.ml_matrix_id);
		shader.upload_matrix(pr, shader.pr_matrix_id);
	}

	/*
		Imgui Components
	*/
	static void init_imgui_content() {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);
		ImGui::StyleColorsDark();
	}

	static void imgui_new_frame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		imgui_setup_menu();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	static void render_layers() {
		for (deque<Layer*>::reverse_iterator it = layers.rbegin(); it != layers.rend(); it++) {
			(*it)->draw();
		}
	}
	static void destroy_window() {
		imgui_destroy();
		glfwTerminate();
		glfwDestroyWindow(window);
	}

private:
	static void imgui_setup_menu() {
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				imgui_setup_menu_file();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				imgui_setup_menu_edit();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Layer"))
			{
				{
					if (ImGui::MenuItem("solid")) {
						Solid *solid = new Solid();
						layers.push_back(solid);
					}
					if (ImGui::BeginMenu("Shape")) {
						if (ImGui::MenuItem("triangle")) {
							Shape *triangle = new Shape(3);
							layers.push_back(triangle);
						}
						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("curves")) {
						/*
						if (ImGui::MenuItem("Bezier Curve")) {
							Point p0(-2.f, -2.f), p1(-2.f, 2.f), p2(2.f, 2.f), p3(2.f, -2.f);
							Bezier *bezier_curve = new Bezier(p0, p1, p2, p3);
							curves.push_back(bezier_curve);
						}
						*/
						ImGui::EndMenu();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Layers");
		{
			int move_from = -1, move_to = -1;
			for (unsigned int i = 0; i < layers.size(); i++) {
				if (ImGui::TreeNode(layers[i]->get_layer_name())) {
					ImGui::TreePop();
				}
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
					ImGui::SetDragDropPayload("DND_DEMO_NAME", &i, sizeof(int));
					ImGui::EndDragDropSource();
				}
				if (ImGui::BeginDragDropTarget()) {
					ImGuiDragDropFlags target_flags = 0;
					target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
					target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME"))
					{
						move_from = *(const int*)payload->Data;
						move_to = i;
					}
					ImGui::EndDragDropTarget();
				}
			}
			if (move_from != -1 && move_to != -1) {
				deque<Layer*>::iterator it = layers.begin() + move_from;
				Layer *cur = *it;
				layers.erase(it);
				it = layers.begin() + move_to;
				layers.insert(it, cur);
			}
		}
		ImGui::End();
	}

	/*
		GLFW WINDWO FUNCTION CALLBACKS
	*/
	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	static void exit_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			xpos -= 1280;
			xpos /= 1280;
			ypos -= 720;
			ypos /= 720;
			// convert to window view port locations
			deque<Layer*>::iterator it = layers.begin();
			if (it != layers.end()) {
				if ((*it)->is_inside_object((float)xpos, (float)ypos)) {
					cout << "inside the object" << endl;
				}
				else {
					cout << "outside the object" << endl;
				}
			}
		}
	}

	/*
		IMGUI COMPONENTS
	*/
	static void imgui_destroy() {
		for (unsigned int i = 0; i < layers.size(); i++) {
			delete layers[i];
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	static void setup_menu_file() {
		ImGui::MenuItem("file menu", NULL, false, false);
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("submenu"))
		{
			ImGui::MenuItem("sub1");
			if (ImGui::BeginMenu("subsub"))
			{
				ImGui::MenuItem("Hello");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Exit", "Alt+F4")) {}
	}

	static void imgui_setup_menu_edit() {
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
		ImGui::Separator();
		if (ImGui::MenuItem("Cut", "CTRL+X")) {}
		if (ImGui::MenuItem("Copy", "CTRL+C")) {}
		if (ImGui::MenuItem("Paste", "CTRL+V")) {}
	}

	static void imgui_setup_menu_file() {
		ImGui::MenuItem("file menu", NULL, false, false);
		if (ImGui::MenuItem("New")) {}
		if (ImGui::MenuItem("Open", "Ctrl+O")) {}
		if (ImGui::BeginMenu("submenu"))
		{
			ImGui::MenuItem("sub1");
			if (ImGui::BeginMenu("subsub"))
			{
				ImGui::MenuItem("Hello");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Exit", "Alt+F4")) {}
	}
};