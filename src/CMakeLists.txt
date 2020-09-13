cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(opengl-demo VERSION 1.0.0)

add_library(glad ${PROJECT_SOURCE_DIR}/../Dependencies/glad/src/glad.c)
link_directories(${PROJECT_SOURCE_DIR}/../Dependencies/GLFW/lib/linux)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(opengl-demo ${SRC_FILES})

target_link_libraries(opengl-demo GL glad glfw3 pthread dl X11)
