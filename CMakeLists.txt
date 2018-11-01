cmake_minimum_required(VERSION 3.9)
project(ifj18)

set(CMAKE_C_STANDARD 99)


set(CMAKE_C_FLAGS "-pedantic -Wall -Wextra")
#add_executable(our_cool_gic top_down_parser.c top_down_parser.h symtable.c symtable.h test.c test.h expressions_parser.c expressions_parser.h error_handle.h error_handle.c dynamic_string.c dynamic_string.h)
add_executable(test test.h test.c dynamic_string.h dynamic_string.c symtable.h symtable.c top_down_parser.h top_down_parser.c expressions_parser.c expressions_parser.h stack.c stack.h semantic_parser.h semantic_parser.c error_handle.c error_handle.h)