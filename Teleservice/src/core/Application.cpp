#include "Application.h"
#include "WindowManager.h"

Application::~Application()
{
	delete MainWindow;
}

void Application::Run()
{
	// OnStart
	WindowManager::StartWindows();

	MSG messages;
	while (GetMessage(&messages, NULL, 0, 0) > 0)
	{
		// OnUpdate
		WindowManager::UpdateWindows();

		// OnRender
		WindowManager::RenderWindows();

		// Handle events
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
}

Window* Application::MainWindow = nullptr;