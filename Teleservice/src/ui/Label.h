#ifndef LABEL_HEADER
#define LABEL_HEADER

#include "Control.h"
#include <string>

class Label : public Control
{
public:
	Label(const std::wstring& text, const int& x, const int& y, const bool& isBold = false) : m_Text(text), m_IsBold(isBold), Control(x, y) {};

	void Create(const HWND& windowHandle) override;

private:
	std::wstring m_Text;
	bool m_IsBold;
};
#endif