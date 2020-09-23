cmake_minimum_required(VERSION 3.2)
project(Arkanoid)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -static")

include_directories(
  "src/audio"
  "src/graphics"
  "src/game"
  "src/input"
  "src/physics"
  "src/utils"
)

file(GLOB SOURCES 
  "src/audio/*.h"
  "src/audio/*.cpp"
  "src/game/*.cpp"
  "src/game/*.h"
  "src/graphics/*.cpp"
  "src/graphics/*.h"
  "src/input/*.cpp"
  "src/input/*.h"
  "src/physics/*.cpp"
  "src/physics/*.h"
  "src/utils/*.cpp"
  "src/utils/*.h"
  "src/*.cpp"
)

# ------------------------
# GLFW build configuration
# ------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(externals/glfw)

# ------------------------

# ------------------------
# GLEW build configuration
# ------------------------

add_subdirectory(externals/glew)
include_directories(externals/glew/include)
add_library(glew STATIC externals/glew/src/glew.c)
add_definitions(-DGLEW_STATIC)

# ------------------------

# ------------------------
# OpenAL build configuration
# ------------------------

add_subdirectory(externals/openal-soft)
include_directories(externals/openal-soft/include)

# ------------------------

# ------------------------
# Header-only libraries
# ------------------------

include_directories(externals/glm)
include_directories(externals/stb)

# ------------------------

add_executable(Arkanoid ${SOURCES})

target_link_libraries(Arkanoid glfw glew OpenAL ${OPENGL_LIBRARY})