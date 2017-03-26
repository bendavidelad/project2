cmake_minimum_required(VERSION 3.6)
project(os2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Thread.cpp Thread.h uthreads.h uthreads.cpp UserLevelManager.cpp UserLevelManager.h main.cpp)
add_executable(os2 ${SOURCE_FILES})