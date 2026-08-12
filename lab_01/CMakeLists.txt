cmake_minimum_required(VERSION 3.29)
project(InfoSec)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc)

add_executable(InfoSec
    inc/enigma.hpp
    inc/mapper.hpp
    inc/reflector.hpp
    inc/rotor.hpp
    src/main.cpp)
