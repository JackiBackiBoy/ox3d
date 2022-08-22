cmake_minimum_required(VERSION 3.20.0)

set(MINGW_PATH C:/msys64/mingw64)
set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/CMake)

include(Utilities)

message(STATUS "Using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
  if (NOT MINGW_PATH)
    message(FATAL_ERROR "MINGW_PATH not set!")
  endif()
  
  set(USE_MINGW "True")
  set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
  set(CMAKE_CXX_COMPILER ${MINGW_PATH}/bin/g++.exe)
endif()

#Set Linker flags
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/sandbox/src)
set(ENGINE_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/oxcart/src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  ${SOURCE_DIR}
  ${CMAKE_HOME_DIRECTORY}/oxcart/src
)

# Retrieve all source and header files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_HOME_DIRECTORY}/sandbox/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_HOME_DIRECTORY}/sandbox/src/*.hpp)
SOURCE_GROUP_BY_FOLDER(SOURCE_FILES)

add_executable(sandbox ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(sandbox PRIVATE
  ${CMAKE_HOME_DIRECTORY}/oxcart/src
)

target_link_libraries(sandbox oxcart)

install(TARGETS sandbox RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin)
