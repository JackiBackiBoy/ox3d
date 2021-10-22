#include "window.h"
#include "resources.h"

Window::Window(const std::wstring& title, const int& width, const int& height, const HINSTANCE& hInstance)
  : m_Title(title), m_Width(width), m_Height(height)
{
  WNDCLASSEXW windowClass = {};
  windowClass.cbSize = sizeof(WNDCLASSEXW);
  windowClass.style = CS_VREDRAW | CS_HREDRAW;
  windowClass.lpfnWndProc = WindowProcedure; 
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(OX3D_ICON));
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
  windowClass.lpszMenuName = NULL;
  windowClass.lpszClassName = L"ox3d";
  windowClass.hIconSm = NULL;
  
  if (!RegisterClassExW(&windowClass))  {
      MessageBoxW(NULL, L"Could not register window class!", L"Error!", MB_OK | MB_ICONEXCLAMATION);
  }
  
  HWND windowHandle = CreateWindowExW(0, // extended styles
                                      L"ox3d", // class name
                                      L"ox3d | Output Window", // window name
                                      WS_OVERLAPPEDWINDOW | WS_VISIBLE, // style
                                      CW_USEDEFAULT, // x
                                      CW_USEDEFAULT, // y
                                      1080, // width
                                      720, // height
                                      NULL, // parent
                                      0, // menu 
                                      hInstance, // instance
                                      0); // lpVoid

}

LRESULT CALLBACK Window::WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_DESTROY)
  {
    PostQuitMessage(0);
  }
  
  return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}