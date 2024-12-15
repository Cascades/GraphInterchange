add_library(GraphInterchange INTERFACE)

target_include_directories(GraphInterchange INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(GraphInterchange INTERFACE /W4 /WX)
