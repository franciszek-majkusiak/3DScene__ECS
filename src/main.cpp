#include "Coordinator.h"
#include "Application/Application.h"
//extern "C"
//{
//	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
//}

int main()
{
	Application app;
	auto version = glGetString(GL_VERSION);
	std::cout << version << std::endl;

	auto text = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << text << std::endl;
	app.Run();
}