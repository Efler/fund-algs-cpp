cmake_minimum_required(VERSION 3.23)
project(allocator_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(allocator_1
        allocator_1.h
        main.cpp)
