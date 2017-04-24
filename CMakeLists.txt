cmake_minimum_required(VERSION 3.6)
project(os2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

<<<<<<< HEAD
set(SOURCE_FILES main.cpp Thread.cpp
        Thread.h UserLevelManager.cpp UserLevelManager.h
        uthreads.cpp uthreads.h)
=======
set(SOURCE_FILES main.cpp Thread.h Thread.cpp UserLevelManager.cpp UserLevelManager.h uthreads.cpp uthreads.h)
>>>>>>> 2122dbec75026d10da187215de57038a72fd9d4f
add_executable(os2 ${SOURCE_FILES})