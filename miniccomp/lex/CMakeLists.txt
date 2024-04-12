cmake_minimum_required(VERSION 3.26)
project(lex)

set(CMAKE_CXX_FLAGS -O2)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_subdirectory(tests)

#add_executable(lex main.cpp src/lexer.cpp include/lexer.h)
add_library(lex src/lexer.cpp)
target_include_directories(lex PUBLIC include)
