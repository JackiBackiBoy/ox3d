#include "MenuItem.h"

MenuItem::MenuItem(const std::wstring& text, MenuItem* parent)
	: m_Text(text), m_Parent(parent), Control(0, 0)
{
	if (m_Parent != nullptr)
	{
		m_Parent->AddItem(this);
	}
}

MenuItem::~MenuItem()
{
	delete m_Parent;
	m_Children.erase(m_Children.begin(), m_Children.end());
}

void MenuItem::Create(const HWND& windowHandle)
{
	if (m_Parent == nullptr)
	{
		m_MenuHandle = CreateMenu();
	}
	else
	{
		if (m_Children.empty())
		{
			AppendMenuW(m_Parent->GetMenuHandle(), MF_STRING, m_ID, m_Text.c_str());
		}
		else
		{
			m_MenuHandle = CreateMenu();
			AppendMenuW(m_Parent->GetMenuHandle(), MF_POPUP, (UINT_PTR)m_MenuHandle, m_Text.c_str());
		}
	}
}

void MenuItem::AddItem(MenuItem* item)
{
	m_Children.push_back(item);
}
