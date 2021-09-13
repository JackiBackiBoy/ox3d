#include "ComboBox.h"
#include "core/ResourceManager.h"

void ComboBox::Create(const HWND& windowHandle)
{
	m_Handle = CreateWindow(L"ComboBox", L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
				m_X, m_Y, m_Width, m_Height, windowHandle, (HMENU)m_ID, NULL, NULL);

	// Loop through all items and add them to the combobox
	for (size_t i = 0; i < m_Items.size(); i++)
	{
		SendMessage(m_Handle, CB_ADDSTRING, NULL, (LPARAM)m_Items[i].c_str());
	}

	SendMessage(m_Handle, CB_SETCURSEL, (WPARAM)1, (LPARAM)0); // set initial display item
	SendMessage(m_Handle, WM_SETFONT, (LPARAM)ResourceManager::GetDefaultFont(false), true);
}

void ComboBox::AddItem(const std::wstring& item)
{
	m_Items.push_back(item);
}

void ComboBox::AddItems(const std::vector<std::wstring>& items)
{
	m_Items.insert(m_Items.end(), items.begin(), items.end());
}

void ComboBox::OnCommand(const HWND& windowHandle, const WPARAM& wParam, const LPARAM& lParam)
{
	WORD wParamMessage = HIWORD(wParam);
	WORD lParamMessage = LOWORD(lParam);

	//if (wParamMessage == CBN_SELCHANGE)
}

