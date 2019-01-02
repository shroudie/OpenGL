#pragma once


#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../layers/solid.h"
#include "../layers/shape.h"
#include "../layers/curves/bezier.h"

#include <deque>

#define GLSL_VERSION "#version 130"

class ImGuiContainers {
public:
	static void new_frame() {
	}
	   




	static void destroy() {
		for (unsigned int i = 0; i < layers.size(); i++) {
			delete layers[i];
		}	
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	//create window function
	static void setup_menu() {
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

	static void render_layers() {
		for (deque<Layer*>::reverse_iterator it = layers.rbegin(); it != layers.rend(); it++) {
			(*it)->draw();
		}
	}

	void set_clicked_object() {
		for (deque<Layer*>::iterator it = layers.begin(); it != layers.end(); it++) {
		}
	}

//private:
	static deque<Layer*> layers;

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
};
