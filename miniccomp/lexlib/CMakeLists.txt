cmake_minimum_required(VERSION 3.26)
project(lexlib)

set(CMAKE_CXX_STANDARD 23)

# header only library
include_directories(include)
add_library(lexlib INTERFACE)
target_include_directories(lexlib INTERFACE include)
