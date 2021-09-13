#ifndef CONTROL_HEADER
#define CONTROL_HEADER

#include "windows.h"

class Control
{
public:
	Control(const int& x, const int& y) : m_X(x), m_Y(y), m_ID(0), m_Handle(0) {};
	virtual ~Control() {};

	virtual void Create(const HWND& windowHandle);
	inline int GetID() const { return m_ID; }
	inline void SetID(const int& id) { m_ID = id; };
	virtual void OnCommand(const HWND& windowHandle, const WPARAM& wParam, const LPARAM& lParam);

protected:
	int m_X;
	int m_Y;
	int m_ID;
	HWND m_Handle;
};
#endif