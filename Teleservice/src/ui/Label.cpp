#include "Label.h"
#include "core/ResourceManager.h"

void Label::Create(const HWND& windowHandle)
{
	SIZE textSize;

	// Trick for getting size of the text to be displayed in pixels
	HFONT hFont = ResourceManager::GetDefaultFont(m_IsBold);
	SendMessage(windowHandle, WM_SETFONT, (LPARAM)hFont, 0);
	HDC hdc = GetDC(NULL);
	SelectObject(hdc, hFont);
	GetTextExtentPoint32(hdc, m_Text.c_str(), m_Text.size(), &textSize);
	DeleteDC(hdc);

	m_Handle = CreateWindow(L"Static", m_Text.c_str(), WS_CHILD | WS_VISIBLE, m_X, m_Y, textSize.cx, textSize.cy, windowHandle, (HMENU)m_ID, NULL, NULL);

	SendMessage(m_Handle, WM_SETFONT, (LPARAM)ResourceManager::GetDefaultFont(m_IsBold), true);
}
