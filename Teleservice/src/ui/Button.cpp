#include "Button.h"
#include "core/ResourceManager.h"

void Button::Create(const HWND& windowHandle)
{
	CreateWindow(L"Button", m_Text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD, m_X, m_Y, m_Width, m_Height, windowHandle, (HMENU)m_ID, NULL, NULL);

	SendDlgItemMessage(windowHandle, m_ID, WM_SETFONT, (LPARAM)ResourceManager::GetDefaultFont(false), true);
}

void Button::OnCommand(const HWND& windowHandle, const WPARAM& wParam, const LPARAM& lParam)
{
	m_OnCommand();
}
