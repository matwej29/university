cmake_minimum_required(VERSION 3.26)
project(lexexe)

set(CMAKE_CXX_STANDARD 23)

add_executable(lexexe main.cpp)
target_link_libraries(lexexe lexlib)

configure_file(example.txt example.txt COPYONLY)

