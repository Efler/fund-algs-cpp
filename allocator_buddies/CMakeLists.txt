cmake_minimum_required(VERSION 3.25)
project(allocator_buddies)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(allocator_buddies
        allocator_buddies.h
        main.cpp)
