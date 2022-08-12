cmake_minimum_required(VERSION 3.20.0)

project(global)

set(MINGW_PATH C:/msys64/mingw64)
set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
set(CMAKE_CXX_COMPILER ${MINGW_PATH}/bin/g++.exe)

find_path(GLFW_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/vendor/glfw)

if ((NOT GLFW_INCLUDE_DIR) OR (NOT EXISTS ${GLFW_INCLUDE_DIR}))
  message(STATUS "Unable to find GLFW...")

  execute_process(COMMAND git submodule update --init -- vendor/glfw
                  WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})

  add_subdirectory(vendor/glfw)
endif()

#add_subdirectory(sandbox)
#add_subdirectory(oxcart
