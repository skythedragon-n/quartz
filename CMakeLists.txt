cmake_minimum_required(VERSION 4.0)
project(quartzrenderer)

add_subdirectory(bgfx.cmake)

set(CMAKE_CXX_STANDARD 23)

add_executable(quartzrenderer src/main.cpp)
