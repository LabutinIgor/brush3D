cmake_minimum_required(VERSION 2.8.8)

set(GLM_PATH ../brush_ui/external/glm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

file(GLOB_RECURSE SOURCE ./src/*.cpp)
file(GLOB_RECURSE HEADER ./src/*.hpp)
file(GLOB_RECURSE INCLUDE_HEADER include/*.hpp)

include_directories(${GLM_PATH})

add_library(Brush STATIC ${SOURCE} ${HEADER} ${INCLUDE_HEADER})
