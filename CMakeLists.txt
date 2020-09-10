cmake_minimum_required(VERSION 3.2)
project(breakout)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static")

set(SOURCES src/main.cpp)

# GLFW build configuration

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(externals/glfw)

# end
# ------------------------
# GLEW build configuration

add_subdirectory(externals/glew)
include_directories(externals/glew/include)
add_library(glew STATIC externals/glew/src/glew.c)
add_definitions(-DGLEW_STATIC)

# end

add_executable(breakout ${SOURCES})

target_link_libraries(breakout glfw glew ${OPENGL_LIBRARY})