#ifndef TEXT_BOX_HEADER
#define TEXT_BOX_HEADER

#include "Control.h"
#include <string>

class TextBox : public Control
{
public:
	TextBox(const int& x, const int& y, const int& width, const int& height) : m_Width(width), m_Height(height), Control(x, y) {};

	void Create(const HWND& windowHandle) override;

private:
	int m_Width;
	int m_Height;
	std::wstring m_Text;
};
#endif