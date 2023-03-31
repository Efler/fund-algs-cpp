cmake_minimum_required(VERSION 3.23)
project(allocator_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(allocator_3
        allocator_3.h
        main.cpp)
