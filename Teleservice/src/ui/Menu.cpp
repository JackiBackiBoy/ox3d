#include "Menu.h"

Menu::~Menu()
{
	m_MenuItems.erase(m_MenuItems.begin(), m_MenuItems.end());
}

void Menu::AddMenuItem(const MenuItem& menuItem)
{
	m_MenuItems.push_back(menuItem);
}

void Menu::Create()
{

}
