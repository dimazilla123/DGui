cmake_minimum_required(VERSION 3.14)

add_library(json STATIC json.c)

add_executable(test_json test_json.c pretty_json.c)
target_link_libraries(test_json json)