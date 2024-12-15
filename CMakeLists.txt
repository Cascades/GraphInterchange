cmake_minimum_required (VERSION 3.29)

project(graph_interchange)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(/W4 /WX)

add_subdirectory(graph_interchange)
add_subdirectory(tests)