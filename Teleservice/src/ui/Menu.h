#ifndef MENU_HEADER
#define MENU_HEADER

#include <vector>
#include "MenuItem.h"
#include "windows.h"

class Menu
{
public:
	Menu() {};
	~Menu();

	void AddMenuItem(const MenuItem& menuItem);
	void Create();

private:
	std::vector<MenuItem> m_MenuItems;
	HMENU m_MenuID;
};
#endif