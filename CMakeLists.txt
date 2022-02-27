cmake_minimum_required(VERSION 3.20.0)

# project name
project(ox3d)

include_directories(${PROJECT_SOURCE_DIR})

# c++ language version
# Set GLFW path for each specific platform
if (WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++") # enable static linking
  set(GLFW_PATH ${PROJECT_SOURCE_DIR}/dependencies/GLFW/windows)
  set(VULKAN_SDK_PATH C:/VulkanSDK/1.3.204.0)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Set VULKAN_SDK_PATH
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
else()
	message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

# 2. Set GLFW_LIB
if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path at: " ${GLFW_PATH})
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64")
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()
if (NOT GLFW_LIB)
  message(STATUS "Could not find glfw library!")
else()
	message(STATUS "Using GLFW lib at: ${GLFW_LIB}")
endif()

#include_directories(GLFW_INCLUDE_DIRS/)
#link_directories(GLFW_INCLUDEDIRS/)

# According to CMAKE documentation this
# operation is not advised, however
# the pros of Globbing significantly
# outweigh the cons of Globbing. 
# The recommended approach by CMAKE
# is to generate a list of all source
# files in the project, however this
# is not needed for the time being.

# Retrieve all source files
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Retrieve all header files
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.h)

add_executable(ox3d ${SOURCE_FILES} ${HEADER_FILES})

# Standard c++ version
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (WIN32)
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
  )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )

  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (APPLE)
  message(STATUS "Creating build for macOS!")
  message(STATUS "Vulkan include dir: " ${Vulkan_INCLUDE_DIRS})
  message(STATUS "Vulkan library dir: " ${Vulkan_LIBRARIES})
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
  )

  target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})
endif()
