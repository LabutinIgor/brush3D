cmake_minimum_required(VERSION 2.6)
project(ObjViewer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")


find_package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(
        external/glfw-3.1.2/include/GLFW/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/tinyobjloader/
        external/Simple-OpenGL-Image-Library/src/
        .
)

add_executable(ObjViewer
        src/main.cpp
        src/load_shaders.h
        src/load_shaders.cpp
        src/MainView.h
        src/MainView.cpp
        external/tinyobjloader/tiny_obj_loader.h
        )

target_link_libraries(ObjViewer ${OPENGL_LIBRARY} glfw GLEW)

