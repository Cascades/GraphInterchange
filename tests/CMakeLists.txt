add_executable(graph_interchange_tests
  main.cpp
)

target_include_directories(graph_interchange_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(graph_interchange_tests PUBLIC GraphInterchange)

target_compile_options(graph_interchange_tests PRIVATE /W4 /WX)