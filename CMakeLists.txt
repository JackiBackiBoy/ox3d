cmake_minimum_required(VERSION 3.20.0)

project(global)

set(MINGW_PATH C:/msys64/mingw64)
set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
set(CMAKE_CXX_COMPILER ${MINGW_PATH}/bin/g++.exe)

add_subdirectory(oxcart)
add_subdirectory(sandbox)
