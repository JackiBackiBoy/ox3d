#ifndef WINDOW_MANAGER_HEADER
#define WINDOW_MANAGER_HEADER

#include "Window.h"
#include <vector>

class WindowManager
{
public:
	~WindowManager() {};

	static void StartWindows();
	static void UpdateWindows();
	static void RenderWindows();
	static void ConstructWindows(const HINSTANCE& hInstance);
	static void AddWindow(Window* window) { m_Windows.push_back(window); }
	static Window* FindWindowByHandle(const HWND& handle);
	inline static Window* GetWindowByIndex(const int& index) { return m_Windows[index]; }

private:
	WindowManager() {};

	static std::vector<Window*> m_Windows;
};
#endif