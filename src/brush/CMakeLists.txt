cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_PREFIX_PATH "/Users/igorl/Qt/5.7/clang_64")

project(Brush)

set(GLM_PATH=../../external/glm)

set(CMAKE_CXX_COMPILER=/usr/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

include_directories(
        GLM_PATH
)

add_library(Brush
        abstractbrush.h
        abstractbrush.cpp
        brushhistory.h
        brushhistory.cpp
        brushstroke.h
        brushstroke.cpp
        pixelsfastbrush.h
        pixelsfastbrush.cpp
        model/brushutils.h
        model/brushutils.cpp
        model/colorchange.h
        model/colorchange.cpp
        model/face.h
        model/face.cpp
        model/geometry.h
        model/geometry.cpp
        model/matrix.h
        model/objectmodel.h
        model/objectmodel.cpp
        )
