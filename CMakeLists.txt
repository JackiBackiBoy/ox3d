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
  set(GLM_PATH ${PROJECT_SOURCE_DIR}/dependencies/GLM)
  set(ASSIMP_PATH ${PROJECT_SOURCE_DIR}/dependencies/assimp/windows)
  set(FREETYPE_PATH ${PROJECT_SOURCE_DIR}/dependencies/freetype/windows)
elseif(APPLE)
  set(FREETYPE_PATH ${PROJECT_SOURCE_DIR}/dependencies/freetype/macOS)
endif()

set(STB_PATH ${PROJECT_SOURCE_DIR}/dependencies/stb)

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

# 2. Find GLFW
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

# 3. Find Assimp
if (DEFINED ASSIMP_PATH)
  set(ASSIMP_INCLUDE_DIR "${ASSIMP_PATH}/include")
  set(ASSIMP_LIBRARIES "${ASSIMP_PATH}/lib")
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

# Retrieve all source files
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Retrieve all header files
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Standard c++ version
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ENGINE_DIR="../"
  NDEBUG
)

if (WIN32)
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_PATH}/include
    ${STB_PATH}
    ${ASSIMP_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR}
  )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
    ${ASSIMP_LIBRARIES}
    ${FREETYPE_LIBRARIES}
  )

  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1 libassimp freetype)
elseif (APPLE)
  message(STATUS "Creating build for macOS!")
  message(STATUS "Vulkan include dir: " ${Vulkan_INCLUDE_DIRS})
  message(STATUS "Vulkan library dir: " ${Vulkan_LIBRARIES})
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${STB_PATH}
    ${ASSIMP_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR}
  )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${FREETYPE_LIBRARIES}
  )

  target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES} ${ASSIMP_LIBRARIES} libfreetype.dylib)
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
  "${PROJECT_SOURCE_DIR}/shaders/*.vert"
  "${PROJECT_SOURCE_DIR}/shaders/*.frag"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
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

# Automatically copying .dll files into the build directory
if (WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_LIBRARIES}/libassimp-5.dll ${PROJECT_SOURCE_DIR}/build
  )
endif()
