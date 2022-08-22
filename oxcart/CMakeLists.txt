cmake_minimum_required(VERSION 3.20.0)

set(NAME oxcart)
set(MINGW_PATH C:/msys64/mingw64)
set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/CMake)

include(Utilities)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "Using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
  if (NOT MINGW_PATH)
    message(FATAL_ERROR "MINGW_PATH not set!")
  endif()
  
  set(USE_MINGW "True")
  set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
  set(CMAKE_CXX_COMPILER ${MINGW_PATH}/bin/g++.exe)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ENGINE_SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/${NAME}/src)
include_directories(
  ${ENGINE_SOURCE_DIR}
  ${CMAKE_HOME_DIRECTORY}/oxcart/src/oxcart/ui
  ${CMAKE_HOME_DIRECTORY}/oxcart/src/oxcart/ui/backends
)

if (WIN32)
  set(GLFW_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/GLFW/windows)
  set(VULKAN_SDK_PATH C:/VulkanSDK/1.3.204.0)
  #set(ASSIMP_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/assimp/windows)
elseif(APPLE)
endif()

set(STB_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/stb)
set(IMGUI_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/imgui)

# 1. GLM
find_path(GLM_INCLUDE_DIR vendor/glm ${CMAKE_HOME_DIRECTORY})

if((NOT GLM_INCLUDE_DIR) OR (NOT EXISTS ${GLM_INCLUDE_DIR}))
  message(STATUS "Unable to find GLM. Installing it now.")
  execute_process(COMMAND git submodule update --init -- ${CMAKE_HOME_DIRECTORY}/vendor/glm
                    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
  
  set(GLM_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/vendor/glm)
else()
  set(GLM_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/vendor/glm)
endif()

# 2. Asset Importer Library
find_path(ASSIMP_INCLUDE_DIR vendor/assimp ${CMAKE_HOME_DIRECTORY})

if((NOT ASSIMP_INCLUDE_DIR) OR (NOT EXISTS ${ASSIMP_INCLUDE_DIR}))
  message(STATUS "Unable to find Assimp. Installing it now.")
  execute_process(COMMAND git submodule update --init -- ${CMAKE_HOME_DIRECTORY}/vendor/assimp
                    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
  
  set(ASSIMP_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/vendor/assimp)
else()
  set(ASSIMP_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/vendor/assimp)
endif()

# 1. Find Vulkan
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

# 2. Find GLFW
if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path at: " ${GLFW_PATH})
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")

  # Check the desired library
  if (MSVC)
    message(STATUS "Using Visual Studio generator")
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2022")
  else()
    message(STATUS "Using MinGW generator")
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64")
  endif()
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()
if (NOT GLFW_LIB)
  message(FATAL_ERROR "Could not find glfw library!")
else()
  message(STATUS "Using GLFW lib at: ${GLFW_LIB}")
endif()

# Retrieve all source and header files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_HOME_DIRECTORY}/${NAME}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_HOME_DIRECTORY}/${NAME}/*.hpp)
SOURCE_GROUP_BY_FOLDER(SOURCE_FILES)

# Export dynamically linked library for the Oxcart engine
add_library(${NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

# Eliminate target prefixes
set_target_properties(${NAME} PROPERTIES PREFIX "")
set_target_properties(${NAME} PROPERTIES IMPORT_PREFIX "")

# Standard c++ version
target_compile_features(${NAME} PUBLIC cxx_std_17)
target_compile_definitions(${NAME} PRIVATE
  OX_BUILD_LIB
  ENGINE_DIR="${CMAKE_HOME_DIRECTORY}/"
  GLM_FORCE_RADIANS
  GLM_FORCE_DEPTH_ZERO_TO_ONE
)

if (WIN32)
  target_include_directories(${NAME} PUBLIC
    ${GLFW_INCLUDE_DIRS}
    ${Vulkan_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${STB_PATH}
    ${IMGUI_PATH}
  )

  target_link_directories(${NAME} PUBLIC
    ${GLFW_LIB}
    ${Vulkan_LIBRARIES}
    ${ASSIMP_LIBRARIES}
  )

  target_link_libraries(${NAME} glfw3 vulkan-1 assimp)
elseif (APPLE)
  message(STATUS "Creating build for macOS!")
  message(STATUS "Vulkan include dir: " ${Vulkan_INCLUDE_DIRS})
  message(STATUS "Vulkan library dir: " ${Vulkan_LIBRARIES})
  target_include_directories(${NAME} PUBLIC
    ${Vulkan_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIR}
    ${STB_PATH}
  )

  target_link_directories(${NAME} PUBLIC
  )

  target_link_libraries(${NAME} glfw ${Vulkan_LIBRARIES} ${ASSIMP_LIBRARIES})
endif()

# ------ Compiling shader files to SPIRV format ------
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)

# Retrieve all vertex and fragment shaders
file (GLOB_RECURSE GLSL_SOURCE_FILES
  "${CMAKE_HOME_DIRECTORY}/assets/shaders/*.vert"
  "${CMAKE_HOME_DIRECTORY}/assets/shaders/*.frag"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${CMAKE_HOME_DIRECTORY}/assets/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
  Shaders
  DEPENDS ${SPIRV_BINARY_FILES}
)

install(TARGETS ${NAME}
  LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
  ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib)
