//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif
//#endif  // _DEBUG

#include "application.h"


int main(void)
{
	Application::init_window_components();
	Application::init_renderer_components();
	Application::init_imgui_content();

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(Application::window))
    {
		Application::clear_window();
		Application::render_layers();
		Application::imgui_new_frame();
		Application::update_window();

		double currentTime = glfwGetTime();
		//cout << currentTime << endl;
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

	}

	Application::destroy_window();
	//_CrtDumpMemoryLeaks();
	::exit(EXIT_SUCCESS);
}