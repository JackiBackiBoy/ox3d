#include "Windows.h"
#include "window.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR lpCmdLine, INT nCmdShow)
{
  Window window = Window(L"ox3d", 1080, 720, hInstance);
  
  MSG messages;
  while (GetMessageW(&messages, NULL, 0, 0) > 0)
  {
      TranslateMessage(&messages);
      DispatchMessage(&messages);
  }
  
  return 0;
}