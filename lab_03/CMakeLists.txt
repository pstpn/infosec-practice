cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

project(lab_03)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/inc)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(app ${SOURCES}
        inc/aes.hpp
        src/aes.cpp
        main.cpp
        inc/pcbc.hpp)

target_include_directories(app PUBLIC ${INCLUDE_DIR})
