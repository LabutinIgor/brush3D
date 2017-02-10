cmake_minimum_required(VERSION 2.8.8)

set(GLM_PATH=../../external/glm)

set(CMAKE_CXX_COMPILER=/usr/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

file(GLOB ROOT_SOURCE ./src/*.cpp)
file(GLOB ROOT_HEADER ./include/*.h)

file(GLOB_RECURSE CORE_SOURCE ./details/*.cpp)
file(GLOB_RECURSE CORE_HEADER ./details/*.h)

include_directories(
        GLM_PATH
        .
)

add_library(Brush STATIC
        ${ROOT_SOURCE} ${ROOT_HEADER}
        ${CORE_SOURCE} ${CORE_HEADER}
        )
