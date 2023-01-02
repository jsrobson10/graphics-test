cmake_minimum_required(VERSION 3.14)

project(Project VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -DGLFW_ENABLED")

file(GLOB_RECURSE SOURCES src/*.c*)

add_executable(BuildLoader tools/build_loader.cpp src/loader/loader.cpp)
target_link_libraries(BuildLoader PUBLIC stdc++ m)

add_custom_target(GenResources ALL ./BuildLoader ../resources > ../src/resources.cpp)
add_executable(Main ${SOURCES})
target_link_libraries(Main PUBLIC stdc++ m glfw OpenGL GLEW)

