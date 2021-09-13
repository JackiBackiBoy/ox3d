#ifndef COMBO_BOX_HEADER
#define COMBO_BOX_HEADER

#include "Control.h"
#include <string>
#include <vector>

class ComboBox : public Control
{
public:
	ComboBox(const int& x, const int& y, const int& width, const int& height, const std::vector<std::wstring>& items)
		: m_Width(width), m_Height(height), m_Items(items), Control(x, y) {};

	void Create(const HWND& windowHandle) override;
	void AddItem(const std::wstring& item);
	void AddItems(const std::vector<std::wstring>& items);
	void OnCommand(const HWND& windowHandle, const WPARAM& wParam, const LPARAM& lParam) override;

private:
	int m_Width;
	int m_Height;
	std::vector<std::wstring> m_Items;
};
#endif