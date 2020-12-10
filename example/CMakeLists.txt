include_directories(${edit_distance_SOURCE_DIR}/include)

macro(add_example name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} ${Boost_LIBRARIES})
    target_compile_features(${name} PRIVATE cxx_auto_type)
endmacro()

add_example(edit_distance_example)
add_example(benchmark)
#add_example(edit_distance_cost_example)
#add_example(edit_distance_range_example)
add_example(edit_script_example)
#add_example(edit_script_cost_example)
#add_example(edit_script_range_example)
#add_example(edit_distance_diff_example)

