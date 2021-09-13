#ifndef MENU_ITEM_HEADER
#define MENU_ITEM_HEADER

#include "Control.h"
#include <string>
#include <vector>

class MenuItem : public Control
{
public:
	MenuItem(const std::wstring& text, MenuItem* parent = nullptr);
	~MenuItem();

	void Create(const HWND& windowHandle) override;
	void AddItem(MenuItem* item);
	inline HMENU GetMenuHandle() const { return m_MenuHandle; }

private:
	HMENU m_MenuHandle;
	std::wstring m_Text;

	MenuItem* m_Parent;
	std::vector<MenuItem*> m_Children;
};
#endif