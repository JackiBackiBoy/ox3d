cmake_minimum_required(VERSION 3.20.0)

if (WIN32)
  set(CMAKE_INSTALL_PREFIX "C:/Users/TheGa/AppData/Local/GLFW")
  set(CMAKE_CXX_COMPILER "C:/Users/TheGa/mingw64/bin/g++.exe")
  set(CMAKE_C_COMPILER "C:/Users/TheGa/mingw64/bin/gcc.exe")
endif()

project(global)

# 1. GLFW
find_path(GLFW_INCLUDE_DIR vendor/glfw ${CMAKE_HOME_DIRECTORY})

if((NOT GLFW_INCLUDE_DIR) OR (NOT EXISTS ${GLFW_INCLUDE_DIR}))
  message(STATUS "Unable to find GLFW. Installing it now.")
  execute_process(COMMAND git submodule update --init -- vendor/glfw
                    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
  set(GLFW_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/vendor/glfw/include)
endif()
set(GLFW_LIBRARIES ${CMAKE_HOME_DIRECTORY}/build/vendor/glfw/src)

# 2. Asset Importer Library
find_path(ASSIMP_INCLUDE_DIR vendor/assimp ${CMAKE_HOME_DIRECTORY})

if((NOT ASSIMP_INCLUDE_DIR) OR (NOT EXISTS ${ASSIMP_INCLUDE_DIR}))
  message(STATUS "Unable to find Assimp. Installing it now.")
  execute_process(COMMAND git submodule update --init -- vendor/assimp
                    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
  
  set(ASSIMP_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/vendor/assimp/include)
endif()
set(ASSIMP_LIBRARIES ${CMAKE_HOME_DIRECTORY}/build/vendor/assimp/lib)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)

# Assimp build flags
set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "enable zlib functionality" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "enable samples functionality" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "enable tests functionality" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "enable tools functionality" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "enable build all imp functionality" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "enable GLTF functionality" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "enable build all exp functionality" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "[Assimp] Treat all warnings as errors." FORCE)

# GLFW build flags
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build GLFW docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
add_subdirectory(vendor/glfw)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)

add_subdirectory(vendor/assimp)
add_subdirectory(oxcart)
add_subdirectory(sandbox)
