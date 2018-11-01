cmake_minimum_required(VERSION 3.9)
project(ifj18 C)

set(CMAKE_C_STANDARD 99)
SET(EXECUTABLE_OUTPUT_PATH ../test_td)

add_executable(test test.c test.h expressions_parser.h expressions_parser.c top_down_parser.c top_down_parser.h
               symtable.h symtable.c stack.c stack.h semantic_parser.c semantic_parser.h
                dynamic_string.h dynamic_string.c error_handle.h error_handle.c)
set(CMAKE_C_FLAGS)
