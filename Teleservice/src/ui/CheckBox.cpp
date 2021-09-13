#include "CheckBox.h"

void CheckBox::Create(const HWND& windowHandle)
{
	m_Handle = CreateWindow(L"Button", m_Text.c_str(), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, m_X, m_X, 185, 35, windowHandle, (HMENU)m_ID, NULL, NULL);
	CheckDlgButton(windowHandle, m_ID, BST_CHECKED);
}

void CheckBox::OnCommand(const HWND& windowHandle, const WPARAM& wParam, const LPARAM& lParam)
{
	m_IsChecked = IsDlgButtonChecked(windowHandle, m_ID);
	CheckDlgButton(windowHandle, m_ID, m_IsChecked ? BST_UNCHECKED : BST_CHECKED);
}
