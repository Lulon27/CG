cmake_minimum_required (VERSION 3.8)

project (CG_Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("src")
add_subdirectory("libs/glfw")
add_subdirectory("libs/glad")
