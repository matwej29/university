cmake_minimum_required(VERSION 3.26)
project(miniccomp)

#set(CMAKE_CXX_FLAGS -O2)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lexlib)
add_subdirectory(lextests)
add_subdirectory(lexexe)
add_subdirectory(parserlib)
add_subdirectory(parserexe)
