# Minimum Cmake version required
cmake_minimum_required(VERSION 3.22)

project(VertexCover VERSION 1.0)

add_executable(vertex_cover ./src/main.c)

target_include_directories(vertex_cover PRIVATE include)

set(CMAKE_C_COMPILER "gcc")

add_compile_options(-g -Wall -Wextra -Werror -pedantic -std=c11 -00)
