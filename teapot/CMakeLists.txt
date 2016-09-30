cmake_minimum_required(VERSION 3.5)
project(teapot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(teapot ${SOURCE_FILES})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
target_link_libraries(teapot ${GLUT_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
target_link_libraries(teapot ${OPENGL_LIBRARIES})

find_package(GLU REQUIRED)
include_directories(${GLU_INCLUDE_DIRS})
target_link_libraries(teapot ${GLU_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(teapot ${GLEW_LIBRARIES})

