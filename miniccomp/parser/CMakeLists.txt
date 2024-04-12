cmake_minimum_required(VERSION 3.26)
project(parser)

set(CMAKE_CXX_FLAGS -O2)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_library(parser src/parser.cpp)
target_link_libraries(parser lex)
target_include_directories(parser PUBLIC include)
