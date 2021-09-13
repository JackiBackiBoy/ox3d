#ifndef BUTTON_HEADER
#define BUTTON_HEADER

#include "Control.h"
#include <string>

class Button : public Control
{
public:
	Button() : Control(0, 0) {};
	Button(const std::wstring& text, const int& x, const int& y, const int& width, const int& height, void(*onCommand)() = []() {})
		   : m_Text(text), m_Width(width), m_Height(height), m_OnCommand(onCommand), Control(x, y) {};

	inline std::wstring GetText() const { return m_Text; }
	inline void SetText(const std::wstring& text) { m_Text = text; }

	void Create(const HWND& windowHandle) override;
	void OnCommand(const HWND& windowHandle, const WPARAM& wParam, const LPARAM& lParam) override;

private:
	std::wstring m_Text;
	int m_Width;
	int m_Height;
	void(*m_OnCommand)();
};
#endif