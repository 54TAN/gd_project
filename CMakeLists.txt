cmake_minimum_required(VERSION 3.5)
project(gd_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

set(SOURCE_FILES main.cpp KdNode.cpp KdNode.h KdTree.cpp KdTree.h Map.cpp Map.h Coordinates.cpp Coordinates.h Obstacle.cpp Obstacle.h Bitmap.cpp Bitmap.h RrtNode.cpp RrtNode.h RrTree.cpp RrTree.h Render.cpp Render.h Consts.h Geometry.h Geometry.cpp Check.h Check.cpp)
add_executable(gd_project ${SOURCE_FILES})

target_link_libraries(gd_project ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
