cmake_minimum_required(VERSION 3.25)
project(red_black_tree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(red_black_tree
        main.cpp
        red_black_tree.h
        debug_rb_tree_printing.h)
