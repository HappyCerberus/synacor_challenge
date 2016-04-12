cmake_minimum_required(VERSION 3.4)
project(synacor_challenge)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp memory.cpp)
add_executable(synacor_challenge ${SOURCE_FILES})