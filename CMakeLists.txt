cmake_minimum_required(VERSION 3.20.0)

# project name
project(ox3d)

include_directories(${PROJECT_SOURCE_DIR})

# c++ language version
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GLFW_PATH C:/Code/C++/ox3d/dependencies/GLFW)
set(VULKAN_SDK_PATH C:/VulkanSDK/1.3.204.0)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Enable static linking
set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")

# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
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

# 2. Set GLFW_PATH in .env.cmake to target specific glfw
if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  if (MSVC)
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2019") # 2.1 Update lib-vc2019 to use same version as your visual studio
    #elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
  else()
    message(STATUS "USING MINGW")
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64") # 2.1 make sure matches glfw mingw subdirectory
  endif()
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()
if (NOT GLFW_LIB)
  message(STATUS "Could not find glfw library!")
else()
	message(STATUS "Using glfw lib at: ${GLFW_LIB}")
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

add_executable(ox3d ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)

# Standard c++ version
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")

  set(MINGW_PATH C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64)
  message(STATUS "MinGW path:" ${MINGW_PATH})
  target_include_directories(${PROJECT_NAME} PUBLIC ${MINGW_PATH}/include)
  target_link_directories(${PROJECT_NAME} PUBLIC ${MINGW_PATH}/lib)

  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
  )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )
endif()
