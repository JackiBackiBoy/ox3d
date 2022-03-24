#pragma once

// Dynamically linked libary export/import defines
#ifdef OX_BUILD_LIB
  #define OX_API __declspec(dllexport)
#else
  #define OX_API __declspec(dllimport)
#endif
