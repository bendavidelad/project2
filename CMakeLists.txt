cmake_minimum_required(VERSION 3.6)
project(os2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Thread.h Thread.cpp UserLevelManager.cpp UserLevelManager.h uthreads.cpp uthreads.h)
add_executable(os2 ${SOURCE_FILES})