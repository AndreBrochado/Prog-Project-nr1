cmake_minimum_required(VERSION 3.1)
project(Projecto1_PROG_GIT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    asd.txt
    Proj1.cpp)

add_executable(Projecto1_PROG_GIT ${SOURCE_FILES})