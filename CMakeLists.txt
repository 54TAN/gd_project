cmake_minimum_required(VERSION 3.6)
project(gd_project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp KdNode.cpp KdNode.h KdTree.cpp KdTree.h)
add_executable(gd_project ${SOURCE_FILES})