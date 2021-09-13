#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#include "core/Window.h"

class MainWindow : public Window
{
public:
	MainWindow(const MainWindow&) = delete;

	static MainWindow* GetInstance() { return m_Instance; }

	void OnCreate() override;
	void OnUpdate() override {};
	void OnRender() override {};
	void OnSize() override;
	void OnActivate() override;

	LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	MainWindow() : Window(L"Teleservice Inkasso", L"TeleserviceInkassoHanterare", 1024, 720) {};

	static MainWindow* m_Instance;

	bool m_IsResizedOrMoved;
};
#endif