cmake_minimum_required(VERSION 3.26)
project(parserexe)

set(CMAKE_CXX_STANDARD 23)

add_executable(parserexe main.cpp)
target_link_libraries(parserexe lexlib parserlib)

configure_file(expr.txt expr.txt COPYONLY)

