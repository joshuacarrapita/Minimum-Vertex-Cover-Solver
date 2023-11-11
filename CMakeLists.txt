cmake_minimum_required(VERSION 3.22)

project(VertexCover VERSION 1.0)

# Set the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add your source files
add_executable(vertex_cover ./src/main.c)

# Choose the compiler
set(CMAKE_C_COMPILER "gcc")

# Specify different compile options for Debug and Release builds
target_compile_options(vertex_cover PRIVATE
    $<$<CONFIG:Debug>: -g -O0 -Wall -Wextra -pedantic -std=c11>
	$<$<CONFIG:Release>: -O3 -march=native -Wall -Wextra -pedantic -std=c11>
)

# Specify different preprocessor definitions for Debug and Release builds
target_compile_definitions(vertex_cover PRIVATE
    $<$<CONFIG:Debug>:DEBUG=1>
)

# Specify different include directories for Debug and Release builds if necessary
target_include_directories(vertex_cover PRIVATE include)
# target_include_directories(your_executable PRIVATE
#     $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/debug_include>
#     $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/release_include>
# )

# Specify different linking options for Debug and Release builds if necessary
# target_link_options(your_executable PRIVATE
#     $<$<CONFIG:Debug>: -L/debug/lib/path>
#     $<$<CONFIG:Release>: -L/release/lib/path>
# )
