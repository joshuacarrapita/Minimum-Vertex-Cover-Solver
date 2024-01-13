##############################################################################
####---------------------------( HELP INFO )------------------------------####
####-----( compile options )                                              ####
#### - compiles to release by default									  ####
#### - to compile to debug or profile use:								  ####
####	- debug: "cmake -DCMAKE_BUILD_TYPE=Debug"						  #### 
####	- profile: "cmake -DCMAKE_BUILD_TYPE=Profile"					  ####
##############################################################################
cmake_minimum_required(VERSION 3.22)

project(UpperBound VERSION 1.0)

# Set the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Additional options for profiling
if(CMAKE_BUILD_TYPE STREQUAL "Profile")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()

# Add your source files
add_executable(upperbound ./src/main.c ./src/graph.c ./src/io.c ./src/bpm.c ./src/clique.c ./include/main.h ./include/graph.h ./include/io.h ./include/bpm.h ./include/clique.h)

# Choose the compiler
set(CMAKE_C_COMPILER "gcc")

# Specify different compile options for Debug and Release builds
target_compile_options(upperbound PRIVATE
    $<$<CONFIG:Debug>: -g -O0 -Wall -Wextra -pedantic -std=c11>
    $<$<CONFIG:Profile>: -pg -O0 -Wall -Wextra -pedantic -std=c11>
	$<$<CONFIG:Release>: -O3 -march=native -Wall -Wextra -pedantic -std=c11>
)

# Specify different preprocessor definitions for Debug and Release builds
target_compile_definitions(upperbound PRIVATE
    $<$<CONFIG:Debug>:DEBUG=1>
)

# Specify different include directories for Debug and Release builds if necessary
target_include_directories(upperbound PRIVATE include)
# target_include_directories(your_executable PRIVATE
#     $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/debug_include>
#     $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/release_include>
# )

# Specify different linking options for Debug and Release builds if necessary
# target_link_options(your_executable PRIVATE
#     $<$<CONFIG:Debug>: -L/debug/lib/path>
#     $<$<CONFIG:Release>: -L/release/lib/path>
# )
