#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "graphics/renderer.h"
#include "layers/solid.h"
#include "layers/shape.h"
#include "layers/customObject.h"
#include "layers/light.h"
#include "layers/curves/bezier.h"
#include <deque>

#define GLSL_VERSION "#version 130"


class Application {
public:
	static GLFWwindow *window;
	static Renderer renderer;

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
		//glfwSetMouseButtonCallback(window, mouse_button_callback); //TODO: 3d
		//glfwSetCursorPosCallback(window, cursor_position_callback);
	}

	static void init_renderer_components() {
		renderer.init_shader("src/shaders/shader.vert", "src/shaders/shader.frag");
		renderer.init_camera(vec3(0.f, 0.f, 16.f));
		renderer.init_uniform_locations();
		//gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		//shader_id = shader.load_shaders("src/shaders/shader.vert", "src/shaders/shader.frag");
		//shader.init_matrix(&shader.pr_matrix_id, "pr_matrix");
		////shader.init_matrix(&shader.ml_matrix_id, "ml_matrix");
		//shader.init_matrix(&shader.vw_matrix_id, "vw_matrix");
		//shader.init_matrix(&shader.un_matrix_id, "uNMatrix");
		//if (enable_3d) {
		//	shader.init_light_locations();
		//}
	}

	//static void upload_matrices() {
	//	int width, height;
	//	glfwGetFramebufferSize(window, &width, &height);
	//	float ratio = width / (float)height;
	//	glViewport(0, 0, width, height);

	//	mat4 pr, vw;
	//	if (enable_3d) {
	//		pr = mat4::perspective_matrix(degreeToRadius(45), ratio, .1f, 500.f);
	//		vw = mat4::look_at(camera.position, camera.position + camera.direction, vec3(0, 1, 0));
	//	}
	//	else {
	//		pr = mat4::orthographic_matrix(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	//	}
	//	mat3 un = mat3::from_mat4(vw);
	//	un = un.transpose();
	//	un = un.invert();

	//	//shader.upload_matrix(ml, shader.ml_matrix_id);
	//	shader.upload_matrix(vw, shader.vw_matrix_id);
	//	//glUniformMatrix3fv(shader.un_matrix_id, 1, GL_FALSE, &un.elements[0]);
	//	shader.upload_matrix(pr, shader.pr_matrix_id);
	//	shader.upload_light_components();
	//}

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
		imgui_setup_control_panel();
		//ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	static void render_layers() {
		renderer.flush();
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
						renderer.submit(solid);
					}
					if (ImGui::BeginMenu("Shape")) {
						if (ImGui::MenuItem("triangle")) {
							Shape *triangle = new Shape(3);
							renderer.submit(triangle);
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
			if (ImGui::TreeNode("camera")) {
				ImGui::PushItemWidth(50); ImGui::DragFloat("posx", &renderer.camera.position[0], 0.05f); ImGui::SameLine();
				ImGui::PushItemWidth(50); ImGui::DragFloat("posy", &renderer.camera.position[1], 0.05f); ImGui::SameLine();
				ImGui::PushItemWidth(50); ImGui::DragFloat("posz", &renderer.camera.position[2], 0.05f);
				ImGui::TreePop();
			}
			int move_from = -1, move_to = -1;
			for (unsigned int i = 0; i < renderer.layers.size(); i++) {
				if (ImGui::TreeNode(renderer.layer_name(i))) {
					ImGui::PushItemWidth(50); ImGui::DragFloat("posx", &renderer.layers[i]->position[0], 0.05f); ImGui::SameLine();
					ImGui::PushItemWidth(50); ImGui::DragFloat("posy", &renderer.layers[i]->position[1], 0.05f); ImGui::SameLine();
					ImGui::PushItemWidth(50); ImGui::DragFloat("posz", &renderer.layers[i]->position[2], 0.05f);
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
				renderer.move(move_from, move_to);
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

	static double cursor_x, cursor_y;
	static bool registered;
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		/*if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				for (selected = layers.begin(); selected != layers.end(); selected++) {
					if ((*selected)->is_inside_object((float)xpos / 640.f - 1.f, 1.f - (float)ypos / 360.f)) {
						cursor_x = xpos;
						cursor_y = ypos;
						registered = true;
						break;
					}
				}
			}
			else {
				cout << "release button" << endl;
				registered = false;
			}
		}*/
	}


	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		/*if (registered) {
			float offsetx = (float)(xpos - cursor_x), offsety = (float)(cursor_y - ypos);
			offsetx /= 640.f;
			offsety /= 360.f;
			(*selected)->move(offsetx, offsety);
			cursor_x = xpos;
			cursor_y = ypos;
		}*/
		//if (selected != layers.end()) {
		//	cout << "should move" << endl;
		//	float offsetx = (float)(xpos - cursor_x), offsety = (float)(ypos - cursor_y);
		//	(*selected)->move(offsetx, offsety);
		//}
	}

	/*
		IMGUI COMPONENTS
	*/
	static void imgui_destroy() {
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
		if (ImGui::MenuItem("Import", "Ctrl+I")) {
			import_custom_object();
		}
		ImGui::Separator();
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

	static void imgui_setup_inlayer_menu() {

	}

	static void imgui_setup_control_panel() {
		ImGui::Begin("Control");
		{
			if (ImGui::Button("Play"))
			{
				renderer.animated = true;
				
			}
			if (ImGui::Button("Pause"))
			{
				renderer.animated = false;
			}
			if (ImGui::Button("Stop"))
			{
				cout << "click onbutton" << endl;
			}
			ImGui::SliderInt("frames ", &renderer.current_frame, 0, 29);
		}
		ImGui::End();
	}

	static void imgui_setup_effect_panel() {
		ImGui::Begin("Visualization");
		{



		}
		ImGui::End();
	}

	static void import_custom_object() {
		customObject *o = new customObject("src/objects/teapot.obj");
		renderer.submit(o);
		o->add_position_key_frame(30, vec3(0, 0, -1.f));
		//o->init_texture_loaded_location(renderer.shader_id);
		o->add_texture("src/textures/texture.png", renderer.shader_id);

		Light *lightsrc = new Light(renderer.shader_id);
		renderer.submit(lightsrc);
		renderer.set_accept_light();
	}
};