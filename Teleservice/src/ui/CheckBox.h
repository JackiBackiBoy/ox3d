#ifndef CHECK_BOX_HEADER
#define CHECK_BOX_HEADER

#include "Control.h"
#include <string>

class CheckBox : public Control
{
public:
	CheckBox(const std::wstring& text, const int& x, const int& y)
		: m_Text(text), Control(x, y) {};

	void Create(const HWND& windowHandle) override;
	void OnCommand(const HWND& windowHandle, const WPARAM& wParam, const LPARAM& lParam) override;

private:
	HWND m_Handle;
	std::wstring m_Text;
	bool m_IsChecked;
};
#endif