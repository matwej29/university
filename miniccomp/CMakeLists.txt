cmake_minimum_required(VERSION 3.26)
project(miniccomp)

set(CMAKE_CXX_FLAGS -O2)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_subdirectory(lex)
add_subdirectory(parser)

add_executable(miniccomp main.cpp)

target_link_libraries(miniccomp lex parser)

