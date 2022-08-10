cmake_minimum_required(VERSION 3.20.0)

project(global)

set(MINGW_PATH C:/msys64/mingw64)

message(STATUS "Using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
  if (NOT MINGW_PATH)
    message(FATAL_ERROR "MINGW_PATH not set!")
  endif()
  
  set(USE_MINGW "True")
  set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
  set(CMAKE_CXX_COMPILER ${MINGW_PATH}/bin/g++.exe)
endif()

add_subdirectory(sandbox)
add_subdirectory(oxcart)
