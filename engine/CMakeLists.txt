cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS TRUE)

set(ENGINE_SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/engine/src)
include_directories(${ENGINE_SOURCE_DIR})

if (WIN32)
  set(GLFW_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/GLFW/windows)
  set(VULKAN_SDK_PATH C:/VulkanSDK/1.3.204.0)
  set(GLM_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/GLM)
  set(ASSIMP_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/assimp/windows)
  set(FREETYPE_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/freetype/windows)
elseif(APPLE)
  set(FREETYPE_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/freetype/macOS)
endif()

set(STB_PATH ${CMAKE_HOME_DIRECTORY}/dependencies/stb)

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
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2019")
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

# 3. Find Assimp
if (DEFINED ASSIMP_PATH)
  set(ASSIMP_INCLUDE_DIR "${ASSIMP_PATH}/include")

  if (MSVC)
    set(ASSIMP_LIBRARIES "${ASSIMP_PATH}/lib-vc2019")
  else()
    set(ASSIMP_LIBRARIES "${ASSIMP_PATH}/lib-mingw-w64")
  endif()

  set(ASSIMP_FOUND "True")
else()
  find_package(ASSIMP REQUIRED)
endif()

if (ASSIMP_FOUND)
  message(STATUS "Found Assimp at: ${ASSIMP_INCLUDE_DIR}")
  message(STATUS "Using Assimp lib at: ${ASSIMP_LIBRARIES}")
else()
  message(STATUS "Could not find Assimp library!")
endif()

# 4. Find Freetype
if (DEFINED FREETYPE_PATH)
  set(FREETYPE_INCLUDE_DIR "${FREETYPE_PATH}/include")
  set(FREETYPE_LIBRARIES "${FREETYPE_PATH}/lib")
  set(FREETYPE_FOUND "True")
else()
  find_package(Freetype REQUIRED)
endif()

if (FREETYPE_FOUND)
  message(STATUS "Found Freetype at: ${FREETYPE_INCLUDE_DIR}")
  message(STATUS "Found Freetype lib at: ${FREETYPE_LIBRARIES}")
else()
  message(STATUS "Could not find Freetype library!")
endif()

# Retrieve all source and header files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_HOME_DIRECTORY}/engine/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_HOME_DIRECTORY}/engine/*.hpp)

# Export dynamically linked library for the Oxcart engine
add_library(engine SHARED ${SOURCE_FILES} ${HEADER_FILES})

# Eliminate target prefixes
set_target_properties(engine PROPERTIES PREFIX "")
set_target_properties(engine PROPERTIES IMPORT_PREFIX "")

# Standard c++ version
target_compile_features(engine PUBLIC cxx_std_17)
target_compile_definitions(engine PRIVATE
  OX_BUILD_LIB
  ENGINE_DIR="${CMAKE_HOME_DIRECTORY}/"
)

if (WIN32)
  target_include_directories(engine PUBLIC
    ${GLFW_INCLUDE_DIRS}
    ${Vulkan_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR}
    ${GLM_PATH}/include
    ${STB_PATH}
  )

  target_link_directories(engine PUBLIC
    ${GLFW_LIB}
    ${Vulkan_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    ${FREETYPE_LIBRARIES}
  )

  target_link_libraries(engine glfw3 vulkan-1 libassimp freetype)
elseif (APPLE)
  message(STATUS "Creating build for macOS!")
  message(STATUS "Vulkan include dir: " ${Vulkan_INCLUDE_DIRS})
  message(STATUS "Vulkan library dir: " ${Vulkan_LIBRARIES})
  target_include_directories(engine PUBLIC
    ${Vulkan_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR}
    ${STB_PATH}
  )

  target_link_directories(engine PUBLIC
    ${FREETYPE_LIBRARIES}
  )

  target_link_libraries(engine glfw ${Vulkan_LIBRARIES} ${ASSIMP_LIBRARIES} libfreetype.dylib)
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

install(TARGETS engine
  LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
  ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib)
