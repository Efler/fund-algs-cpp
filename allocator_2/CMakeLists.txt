cmake_minimum_required(VERSION 3.23)
project(allocator_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(allocator_2
        abstract_allocator.h
        allocator_2.h
        main.cpp)
