#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "Windows.h"

class Window {
  public:
    Window(const std::wstring& title, const int& width, const int& height, const HINSTANCE& hInstance);
    
  private:
    static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    std::wstring m_Title;
    int m_Width;
    int m_Height;
};
#endif
