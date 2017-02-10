cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_PREFIX_PATH "/Users/igorl/Qt/5.7/clang_64")

project(Brush)

set(GLM_PATH=../../external/glm)

set(CMAKE_CXX_COMPILER=/usr/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

include_directories(
        GLM_PATH
        .
)

add_library(Brush
        include/abstractbrush.h
        include/brushstroke.h
        include/pixelsfastbrush.h
        include/matrix.h
        include/objectmodel.h
        include/colorchange.h
        src/abstractbrush.cpp
        src/brushstroke.cpp
        src/pixelsfastbrush.cpp
        src/objectmodel.cpp
        src/colorchange.cpp
        details/brushutils.h
        details/brushutils.cpp
        details/face.h
        details/face.cpp
        details/geometry.h
        details/geometry.cpp
        )
