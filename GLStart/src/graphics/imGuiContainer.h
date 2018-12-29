#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../layers/solid.h"

#define GLSL_VERSION "#version 130"

class ImGuiContainers {
public:
	ImGuiContainers() {}
	~ImGuiContainers();

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
				{
					if (ImGui::MenuItem("solid")) {
						vector<GLfloat> vertex = {
							-1.f, -1.f, -5.f,
							1.f, -1.f, -5.f,
							0.f, 1.f, -5.f
						};
						float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
						vector<GLfloat> color;
						if (solids.size() % 2 == 0) {
							color = {
								r, 0.f, 0.f,
								r, 0.f, 0.f,
								r, 0.f, 0.f
							};
						}
						else {
							color = {
								0.f, 0.f, r,
								0.f, 0.f, r,
								0.f, 0.f, r
							};
						}
						Solid *s = new Solid(vertex, color);
						char name[100];
						strcpy(name, "layers");
						strcat(name, to_string(counter).c_str());
						s->set_layer_name(name);
						solids.push_back(s);
						storage.push_back(counter);
						cout << storage[counter] << endl;
						counter += 1;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Layers");
		{
			int move_from = -1, move_to = -1;
			for (unsigned int i = 0; i < solids.size(); i++) {
				if (ImGui::TreeNode(solids[i]->get_layer_name())) {
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
				vector<Solid*>::iterator it = solids.begin() + move_from;
				Solid *cur = *it;
				solids.erase(it);
				it = solids.begin() + move_to;
				solids.insert(it, cur);
			}
		}
		ImGui::End();
	}

	void render_layers() {
		for (vector<Solid*>::reverse_iterator it = solids.rbegin(); it != solids.rend(); it++) {
			(*it)->draw();
		}
	}

private:
	vector<int> storage;
	vector<Solid*> solids;
	int counter = 0;

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

	static void setup_menu_edit() {
		if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
		if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
		ImGui::Separator();
		if (ImGui::MenuItem("Cut", "CTRL+X")) {}
		if (ImGui::MenuItem("Copy", "CTRL+C")) {}
		if (ImGui::MenuItem("Paste", "CTRL+V")) {}
	}

	static void setup_menu_layer() {

	}
};
