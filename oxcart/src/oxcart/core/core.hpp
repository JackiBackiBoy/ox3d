#pragma once

// Dynamically linked library export/import defines
#ifdef WIN32
  #ifdef OX_BUILD_LIB
    #define OX_API __declspec(dllexport)
  #else
    #define OX_API __declspec(dllimport)
  #endif
#else
  #define OX_API
#endif
