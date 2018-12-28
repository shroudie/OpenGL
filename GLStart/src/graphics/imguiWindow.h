#pragma once

#include <GLFW/glfw3.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../layers/solid.h"

#define GLSL_VERSION "#version 130"


/*
	Support for layers
*/
struct Node {
	void *data;
	Node *next;
	Node(void *d, Node *next = NULL) {
		this->data = d;
		this->next = next;
	}
};

class Linkedlist {
public:
	Linkedlist() {}
	~Linkedlist() {

	}
	void insert_front(void *d) {
		if (head) {
			Node *tmp = head;
			head = new Node(d, tmp);
		}
		else {
			head = new Node(d);
		}
	}

private:
	Node *head = NULL;
};

class ImGuiWindow {
public:
	ImGuiWindow() { counter = 0; }

	static void new_frame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	static void init_window_content(GLFWwindow *win) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui_ImplGlfw_InitForOpenGL(win, true);
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);
		ImGui::StyleColorsDark();
	}

	static void render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	static void destroy() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	//create window function
	void setup_menu() {
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				setup_menu_file();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				setup_menu_edit();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Layer"))
			{
				setup_menu_layer();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		static float f = 0.f;
		ImGui::Begin("Layers");

		if (ImGui::TreeNode("camera")) {
			ImGui::Text("position"); ImGui::SameLine();
			ImGui::DragFloat("float", &f, 0.05f);
			ImGui::TreePop();
		}
		ImGui::End();
	}

private:
	Linkedlist l;
	int counter;

	void setup_menu_file() {
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

	void setup_menu_edit() {
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
		ImGui::Separator();
		if (ImGui::MenuItem("Cut", "CTRL+X")) {}
		if (ImGui::MenuItem("Copy", "CTRL+C")) {}
		if (ImGui::MenuItem("Paste", "CTRL+V")) {}
	}


	void setup_menu_layer() {
		if (ImGui::MenuItem("solid")) {
			vector<GLfloat> vertex{
			   -1.0f, -1.0f, -5.0f,
			   1.0f, -1.0f, -5.0f,
			   0.0f,  1.0f, -5.0f,
			};
			vector<GLfloat> color;
			if (!counter) {
				color = vector<GLfloat> {
				   1.0f, 0.f, 0.0f,
				   1.0f, 0.f, 0.f,
				   1.0f, 0.f, 0.f,
				};
				counter += 1;
			}
			else {
				color = vector<GLfloat>{
				   0.f, 0.f, 1.0f,
				   0.f, 0.f, 1.f,
				   0.f, 0.f, 1.f,
				};
			}
			Solid triangle(vertex, color);
			triangle.bind();
		}
	}
};
