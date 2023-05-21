cmake_minimum_required(VERSION 3.25)
project(avl_tree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(avl_tree
        avl_tree.h
        main.cpp debug_avl_tree_printing.h)
