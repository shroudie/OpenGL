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
	Application::init_shader_components();
	Application::init_imgui_content();

    while (!Application::should_close())
    {
		Application::clear_window();
		Application::upload_matrices();
		Application::render_layers();
		Application::imgui_new_frame();
		Application::update_window();
	}

	Application::destroy_window();
	//_CrtDumpMemoryLeaks();
	::exit(EXIT_SUCCESS);
}