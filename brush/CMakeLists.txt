cmake_minimum_required(VERSION 2.8.8)

set(GLM_PATH=../../external/glm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

file(GLOB_RECURSE SOURCE ./src/*.cpp)
file(GLOB_RECURSE HEADER ./src/*.hpp)
file(GLOB_RECURSE INCLUDE_HEADER include/*.hpp)

add_subdirectory(../brush_ui/external/glm CMAKE_CURRENT_BINARY_DIR)

include_directories(
        ../brush_ui/external/glm)

add_library(Brush STATIC
        ${SOURCE} ${HEADER} ${INCLUDE_HEADER})
