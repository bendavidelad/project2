cmake_minimum_required(VERSION 3.6)
project(os2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test132.cpp Thread.cpp Thread.h UserLevelManager.cpp UserLevelManager.h uthreads.h
        uthreads.cpp)
add_executable(os2 ${SOURCE_FILES})