#include "WindowManager.h"
#include <algorithm>

void WindowManager::StartWindows()
{
	for (size_t i = 0; i < m_Windows.size(); i++)
	{
		m_Windows[i]->OnStart();
	}
}

void WindowManager::UpdateWindows()
{
	for (size_t i = 0; i < m_Windows.size(); i++)
	{
		m_Windows[i]->OnUpdate();
	}
}

void WindowManager::RenderWindows()
{
	for (size_t i = 0; i < m_Windows.size(); i++)
	{
		m_Windows[i]->OnRender();
	}
}

void WindowManager::ConstructWindows(const HINSTANCE& hInstance)
{
	for (size_t i = 0; i < m_Windows.size(); i++)
	{
		m_Windows[i]->ConstructWindow(hInstance);
	}
}

Window* WindowManager::FindWindowByHandle(const HWND& handle)
{
	auto result = std::find_if(m_Windows.begin(), m_Windows.end(), [&](const auto& w) { return w->GetHandle() == handle; });
	
	if (result == m_Windows.end()) // no window with the given handle was found
	{
		return nullptr;
	}

	return *result;
}

std::vector<Window*> WindowManager::m_Windows;
