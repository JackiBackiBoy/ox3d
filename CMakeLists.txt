cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_COMPILER "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/g++.exe")
set(CMAKE_C_COMPILER "C:/ProgramData/chocolatey/lib/mingw/tools/install/mingw64/bin/gcc.exe")

project(global)

set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "enable zlib functionality")
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "enable samples functionality")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "enable tests functionality")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "enable tools functionality")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "enable build all imp functionality")
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "enable GLTF functionality")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "enable build all exp functionality")
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "[Assimp] Treat all warnings as errors.")

add_subdirectory(vendor/assimp)
add_subdirectory(oxcart)
add_subdirectory(sandbox)
