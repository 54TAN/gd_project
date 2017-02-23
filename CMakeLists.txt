cmake_minimum_required(VERSION 3.6)
project(gd_project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp KdNode.cpp KdNode.h KdTree.cpp KdTree.h Map.cpp Map.h Point.cpp Point.h Obstacle.cpp Obstacle.h Bitmap.cpp Bitmap.h RrtNode.cpp RrtNode.h RrTree.cpp RrTree.h Render.cpp Render.h)
add_executable(gd_project ${SOURCE_FILES})