cmake_minimum_required(VERSION 3.25)
project(binary_search_tree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(binary_search_tree
        associative_container.h
        binary_search_tree.h
        main.cpp)
