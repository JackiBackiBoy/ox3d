#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "Window.h"

class Application
{
public:
	Application() {};
	~Application();

	void Run();

	static Window* MainWindow;
};
#endif