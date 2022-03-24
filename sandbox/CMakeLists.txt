cmake_minimum_required(VERSION 3.20.0)

#Set Linker flags
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_DIR ${CMAKE_HOME_DIRECTORY}/sandbox/src)
set(ENGINE_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/engine/src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  ${SOURCE_DIR}
  ${CMAKE_HOME_DIRECTORY}/engine/src
)

# Retrieve all source and header files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_HOME_DIRECTORY}/sandbox/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_HOME_DIRECTORY}/sandbox/src/*.h)

add_executable(sandbox ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(sandbox PRIVATE
  ${CMAKE_HOME_DIRECTORY}/engine/src
)

target_link_libraries(sandbox engine)

install(TARGETS sandbox RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin)
