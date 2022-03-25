#pragma once

// Dynamically linked libary export/import defines
#ifdef OX_BUILD_LIB
  #ifdef WIN32
    #define OX_API __declspec(dllexport)
  #else
    #define OX_API
  #endif
#else
  #ifdef WIN32
    #define OX_API __declspec(dllimport)
  #else
    #define OX_API
  #endif
#endif
