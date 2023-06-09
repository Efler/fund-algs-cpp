cmake_minimum_required(VERSION 3.25)
project(big_int)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(big_int
        abstract_big_int.h
        main.cpp big_int_concrete.h)
