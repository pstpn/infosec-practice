cmake_minimum_required(VERSION 3.29)
project(InfoSec)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc)

add_executable(InfoSec
        main.cpp
        inc/des.hpp
        inc/cfb.hpp
    )