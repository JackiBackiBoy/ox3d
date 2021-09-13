#include "TextBox.h"
#include "core/ResourceManager.h"

void TextBox::Create(const HWND& windowHandle)
{
	m_Handle = CreateWindow(L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
							m_X, m_Y, m_Width, m_Height, windowHandle, (HMENU)m_ID, NULL, NULL);

	SendMessage(m_Handle, WM_SETFONT, (LPARAM)ResourceManager::GetDefaultFont(false), true);
}
