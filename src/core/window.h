#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <stdint.h>

class Window {
  public:
    Window(const uint32_t& width, const uint32_t& height)
      : m_Width(width), m_Height(height) {}

    void run();

  private:
    uint32_t m_Width;
    uint32_t m_Height;
};
#endif
