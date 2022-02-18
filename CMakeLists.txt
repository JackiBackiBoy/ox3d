cmake_minimum_required(VERSION 3.2)

# project name
project(ox3d)

include_directories(${PROJECT_SOURCE_DIR})

# c++ language version
set(CMAKE_CXX_FLAGS "-std=c++11") # c++11

# According to CMAKE documentation this
# operation is not advised, however
# the pros of Globbing significantly
# outweigh the cons of Globbing. 
# The recommended approach by CMAKE
# is to generate a list of all source
# files in the project, however this
# is not needed for the time being.

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Retrieve all source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp")

# Retrieve all heade files
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.hpp")

add_executable(ox3d ${SOURCE_FILES} ${HEADER_FILES})
