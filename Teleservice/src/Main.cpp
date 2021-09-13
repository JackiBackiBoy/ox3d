#include "core/Application.h"
#include "core/ResourceManager.h"
#include "core/WindowManager.h"
#include "windows/MainWindow.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	ResourceManager::Initialize();
	WindowManager::ConstructWindows(hInstance);

	Application* application = new Application();
	Application::MainWindow = MainWindow::GetInstance();

	application->Run();

	delete application;
	return 0;
}