cmake_minimum_required(VERSION 3.26)
project(parserlib)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_library(parserlib src/parser.cpp src/translator.cpp)
target_link_libraries(parserlib lexlib)
target_include_directories(parserlib PUBLIC include)
